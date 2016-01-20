#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/******************************************************************************
 * SUDOKU ./reader.c                                                          *
 * Steuerung für die Händische Manipulation von Sudokus.
 * @author Felix Schütze, dhbw@felix-schuetze.de
 ******************************************************************************/
#if !defined(WIN32)
int x = 3, y = 2;
#include <termios.h>
#include <sys/ioctl.h>
/*
 * Erhaelt ein char, ohne, dass man hierfür Enter drücken muss
 */
int getch() {
	static int ch = -1, fd = 0;
	struct termios neu, alt;
	fd = fileno(stdin);
	tcgetattr(fd, &alt);
	neu = alt;
	neu.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd, TCSANOW, &neu);
	ch = getchar();
	tcsetattr(fd, TCSANOW, &alt);
	return ch;
}

/*
 * Prüft, wie viele Konsolenspalten zur Verfügung stehen
 */
int getSpalten() {
	struct winsize max;
	ioctl(0, TIOCGWINSZ, &max);
	return max.ws_col;
}

/*
 * Verschiebt den Cursor nach x,y
 */
void gotoxy(int x, int y) {
	printf("%c[%d;%df", 0x1B, y, x);
}

/*
 * Printet ein char farbig
 */
void highlightGrun(char c) {
	printf("\033[32m%c\033[0m", c);
}

void highlightRot(char c) {
	printf("\033[31m%c\033[0m", c);
}

#define CLEAR "clear"
#define UP 'A'
#define DOWN 'B'
#define RIGHT 'C'
#define LEFT 'D'
#else
#include <conio.h>
#include <windows.h>
int x = 2, y = 1;
/*
 * Verschiebt den Cursor nach x,y
 */
void gotoxy(int x, int y) {
	//SetCursorPos(x,y);
	COORD c= {x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

/*
 * Printet ein char farbig
 */
void highlightGrun(char c) {
	wprintf("\033[32m%c\033[0m", c);
}

void highlightRot(char c) {
	wprintf("\033[31m%c\033[0m", c);
}

/*
 * Prüft, wie viele Konsolenspalten zur Verfügung stehen
 */
int getSpalten() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.srWindow.Right - csbi.srWindow.Left + 1;

}

#define CLEAR "cls"
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#endif
#define LEGENDE 13
int setFeld(int x, int y, int eingabe, int lock);
int legende = 0;

/*
 * Gibt eine Nachricht unterhalb des Spielfeldes aus
 */
void meldungAusgeben(char* nachricht) {
	//system(CLEAR);
	//printFeld();
	gotoxy(1, HOEHE * 2 + 2);
	printf("%s", nachricht);
	gotoxy(x, y);
}

/*
 * Diese Methode macht gar nichts spezielles, einfach ignorieren!
 */
char* getJoke() {
	time_t t;
	time(&t);
	srand((unsigned int) t);
	switch (rand() % 7) {
	case 0:
		return "Welches Tier kann addieren? -- Ein Oktoplus.";
	case 1:
		return "Um Rekursion zu verstehen, muss man zunächst Rekursion verstehen.";
	case 2:
		return "w.z.b.w. := was zu bezweifeln wäre";
	case 3:
		return "Die meisten Menschen haben überdurchschnittlich viele Beine!";
	case 4:
		return "Was hat Windows mit einem U-Boot gemeinsam? Kaum macht man ein Fenster auf, fangen die Probleme an.";
	case 5:
		return "Wieviele Programmierer braucht man, um eine Glühbirne zu wechseln? Keinen, ist ein Hardware-Problem!";
	default:
		return "Wie nennt man ein Rind mit Root-Rechten? Sudo ku";
	}
}

/*
 * Diese Methode gibt einen Wert im Sudoku als Druckbares Zeichen zurück.
 */
char asFeld(char c) {
	if (c == 0) {
		return ' ';
	}
	return c + 48;
}

/*
 * Diese Methode schreibt in den standartout eine gut lesbare Darstellung des Sudokus
 */
void printFeld() {
	system(CLEAR);
	if (getSpalten() < BREITE * 4 + 1) {
		printf("Nicht genug Platz vorhanden, das ist ein Fehler!");
		return;
	}
	int i, j;
	for (i = 0; i < HOEHE; i++) {
		for (j = 0; j < BREITE; j++) {
			printf("%s%s",
					j == 0 ? i == 0 ? "╔" : i % KACHELHOEHE != 0 ? "╟" : "╠"
					:
					(j % KACHELBREITE == 0 && i != 0) ?
							i % KACHELHOEHE == 0 ? "╬" : "╫"
					:
					(i % KACHELHOEHE != 0) ? "┼" :
					i != 0 ? "╪" : j % KACHELBREITE != 0 ? "\u2564" : "╦",
					(i % KACHELHOEHE == 0) ? "═══" : "───");
		}
		printf("%s\n", (i != 0) ? i % KACHELHOEHE != 0 ? "\u2562" : "╣" : "╗");
		for (j = 0; j < BREITE; j++) {
			printf("%s ", (j % KACHELBREITE == 0) ? "║" : "│");
			if (schutz[i][j] <= 0) {
				if (schutz[i][j] >= 0) {
					printf("%c ", asFeld(feld[i][j]));
				} else {
					highlightRot(asFeld(feld[i][j]));
					printf(" ");
				}
			} else {
				highlightGrun(asFeld(feld[i][j]));
				printf(" ");
			}
		}
		printf("║     ");
		char *hilfe[LEGENDE] = { "Pfeiltasten/wasd: Cursor bewegen",
				"1-9: Zahl eintragen", "Leerzeichen: Zahl löschen",
				"k: Spiel prüfen", "l: Spiel lösen", "p: Spiel speichern",
				"o: Spiel laden", "c: Spiel leeren", "m: Seite weiter",
				"x: Schreibschutz aufheben", "g: Sudoku generieren", "u: Über",
				"q: Programm beenden", "n: Seite zurück" };
		int nummer = legende * HOEHE + i;
		if (nummer < LEGENDE && getSpalten() > BREITE * 4 + 40) {
			printf("%s", hilfe[nummer]);
		}

		printf("\n");

	}
	for (j = 0; j < BREITE; j++) {
		printf("%s%s", j == 0 ? "╚" : j % KACHELBREITE != 0 ? "╧" : "╩",
				(i % KACHELHOEHE == 0) ? "═══" : "   ");
	}
	printf("╝\n");
}

/*
 * Zeigt eine Infoseite auf dem Bildschirm an
 */
void printUber(int y) {
	static int offen = 0;
	if (offen) {
		offen = 0;
		printFeld();
	} else {
		offen = 1;
		system(CLEAR);
		printf(" _______           _______  _______  _______ \n");
		printf("(  ____ \\|\\     /|(  ____ )(  ____ \\(  ____ )\n");
		printf("| (    \\/| )   ( || (    )|| (    \\/| (    )|\n");
		printf("| (_____ | |   | || (____)|| (__    | (____)|\n");
		printf("(_____  )| |   | ||  _____)|  __)   |     __)\n");
		printf("      ) || |   | || (      | (      | (\\ (   \n");
		printf("/\\____) || (___) || )      | (____/\\| ) \\ \\__\n");
		printf("\\_______)(_______)|/       (_______/|/   \\__/\n\n");
		printf(" _______           ______   _______  _                \n");
		printf(
				"(  ____ \\|\\     /|(  __  \\ (  ___  )| \\    /\\|\\     /|\n");
		printf("| (    \\/| )   ( || (  \\  )| (   ) ||  \\  / /| )   ( |\n");
		printf("| (_____ | |   | || |   ) || |   | ||  (_/ / | |   | |\n");
		printf("(_____  )| |   | || |   | || |   | ||   _ (  | |   | |\n");
		printf("      ) || |   | || |   ) || |   | ||  ( \\ \\ | |   | |\n");
		printf("/\\____) || (___) || (__/  )| (___) ||  /  \\ \\| (___) |\n");
		printf(
				"\\_______)(_______)(______/ (_______)|_/    \\/(_______)\n\n\n");
		printf("Entwickelt von:\n");
		printf("Koch, Moritz\n");
		printf("Scherrer, Sascha\n");
		printf("Schütze, Felix\n\n\n");
		if (y)
			printf("u: Zurück");
	}

}

/*
 * Empfaengt Tastatureingaben und steuert mit diesen das Spielfeld
 */
int eingabeLoop() {
	int i, j;
	printFeld();
	gotoxy(x, y);
	while (1) {
		char tmp = getch();
		switch (tmp) {
		case 'w': // Kästchen hoch
		case UP:
			if (y >= 4)
				y -= 2;
			break;
		case 'a': // Kästchen nach links
		case LEFT:
			if (x >= 7)
				x -= 4;
			break;
		case 's': // Kästchen nach unten
		case DOWN:
			if (y < HOEHE * 2)
				y += 2;
			break;
		case 'd': // Kästchen nach rechts
		case RIGHT:
			if (x < BREITE * 4 - 2)
				x += 4;
			break;
		case 'q': // Beenden
			system(CLEAR);
			exit(0);
			break;
		case 'p': { // Speichern
			printFeld();
			meldungAusgeben("Speicherort eingeben:");
			gotoxy(1, HOEHE * 2 + 3);
			char string[100];
			scanf("%99s", &string[0]);
			speichereFeld(string);
			printFeld();
			meldungAusgeben("Gespeichert");
		}
			break;
		case 'o': { // Öffnen
			printFeld();
			meldungAusgeben("Speicherort eingeben:");
			gotoxy(1, HOEHE * 2 + 3);
			char string[100];
			scanf("%99s", &string[0]);
			if (!leseFeldAusDatei(string)) {
				printFeld();
				meldungAusgeben("Laden der Datei fehlgeschlagen");
			} else {
				system(CLEAR);
				printFeld();
			}
		}
			break;
		case 'l': // Sudoku lösen
			printFeld();
			//
			loeseSudokuMain();
			printFeld();
			break;
		case 'g': // Sudoku generieren
			generiereSudoku(feld);
			printFeld();
			break;
		case 'k': // Sudoku überprüfen (Ist es formal korrekt?)
			//printFeld();
			//gotoxy(1, HOEHE * 2 + 3);
			if (!checkSudokuFormal()) {
				printFeld();
				meldungAusgeben("Sudoku ist korrekt");
			} else {
				printFeld();
				meldungAusgeben("Sudoku ist nicht korrekt");
			}
			break;
		case 'n': // Anzeige der Befehlsoptionen: Vor
			if (legende > 0)
				legende--;
			printFeld();
			break;
		case 'm': // Anzeige der Befehlsoptionen: Zurück
			if (legende < (LEGENDE / HOEHE))
				legende++;
			printFeld();
			break;
		case 'c': // Sudoku leeren (Setzt Feld und Schutz zurück)
			for (i = 0; i < BREITE; i++) {
				for (j = 0; j < HOEHE; j++) {
					feld[i][j] = 0;
					schutz[i][j] = 0;
				}
			}
			printFeld();
			break;
		case 'x': // Schutz aufheben
			for (i = 0; i < BREITE; i++) {
				for (j = 0; j < HOEHE; j++) {
					/* schutz > 0 ist lock, schutz < 0 ist fehler */
					if(schutz[i][j] > 0)
						schutz[i][j] = 0;
				}
			}
			printFeld();
			break;
		case 'u': // Über: Zeigt eine Info über das Programm an
			printUber(1);
			break;
		case 'j': // Macht nichts besonderese :)
			printFeld();
			meldungAusgeben(getJoke());
			break;
		default:
			if (tmp >= '1' && tmp <= '9') {
				/*
				 * Die Zählung der Position beginnt bei Windows anders
				 */
#if !defined(WIN32)
				setFeld((y - 2) / 2, (x - 3) / 4, tmp - '0', 0);
#else
				setFeld((y - 1) / 2, (x - 2) / 4, tmp - '0', 0);
#endif
				system(CLEAR);
				printFeld();
			}
			if (tmp == ' ') {
				setFeld((y - 2) / 2, (x - 3) / 4, 0, 0);
				system(CLEAR);
				printFeld();
			}
		}
		gotoxy(x, y);
	}
}
