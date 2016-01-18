#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/******************************************************************************
 * SUDOKU ./reader.c                                                          *
 * Steuerung für die Händische Manipulation von Sudokus.
 * @author Felix Schütze, dhbw@felix-schuetze.de
 ******************************************************************************/
#if !defined(WIN32)
#include <termios.h>
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
	printf("\033[32m%c\033[0m", c);
}

void highlightRot(char c) {
	printf("\033[31m%c\033[0m", c);
}

#define CLEAR "cls"
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#endif
#define LEGENDE 12
int setFeld(int x, int y, int eingabe, int lock);
int x = 3, y = 2, legende = 0;

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
					i != 0 ? "╪" : j % KACHELBREITE != 0 ? "┯" : "╦",
					(i % KACHELHOEHE == 0) ? "═══" : "───");
		}
		printf("%s\n", (i != 0) ? i % KACHELHOEHE != 0 ? "┨" : "╣" : "╗");
		for (j = 0; j < BREITE; j++) {
			printf("%s ", (j % KACHELBREITE == 0) ? "║" : "│");
			if (!schutz[i][j]) {
				if (feld[i][j] >= 0) {
					printf("%c ", asFeld(feld[i][j]));
				} else {
					highlightRot(asFeld(feld[i][j] * -1));
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
				"l: Spiel lösen", "k: Spiel prüfen", "p: Spiel speichern",
				"o: Spiel laden", "c: Spiel leeren", "n/m: Seite vor/zurück",
				"x: Schreibschutz aufheben", "u: Über", "q: Programm beenden" };
		int nummer = legende * HOEHE + i;
		if (nummer < LEGENDE) {
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
		case 'w':
		case UP:
			if (y >= 4)
				y -= 2;
			break;
		case 'a':
		case LEFT:
			if (x >= 7)
				x -= 4;
			break;
		case 's':
		case DOWN:
			if (y < HOEHE * 2)
				y += 2;
			break;
		case 'd':
		case RIGHT:
			if (x < BREITE * 4 - 2)
				x += 4;
			break;
		case 'q':
			system(CLEAR);
			exit(0);
			break;
		case 'p': {
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
		case 'o': {
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
		case 'l':
			printFeld();
			//
			loeseSudokuMain();
			printFeld();
			break;
		case 'k':
			printFeld();
			gotoxy(1, HOEHE * 2 + 3);
			if (!checkSudokuFormal()) {
				meldungAusgeben("Sudoku ist korrekt");
			} else {
				meldungAusgeben("Sudoku ist nicht korrekt");
			}
			break;
		case 'n':
			if (legende > 0)
				legende--;
			printFeld();
			break;
		case 'm':
			if (legende < (LEGENDE / HOEHE))
				legende++;
			printFeld();
			break;
		case 'c':
			for (i = 0; i < BREITE; i++) {
				for (j = 0; j < HOEHE; j++) {
					feld[i][j] = 0;
					schutz[i][j] = 0;
				}
			}
			printFeld();
			break;
		case 'x':
			for (i = 0; i < BREITE; i++) {
				for (j = 0; j < HOEHE; j++) {
					schutz[i][j] = 0;
				}
			}
			printFeld();
			break;
		case 'u':
			printUber(1);
			break;
		case 'j':
			printFeld();
			meldungAusgeben(getJoke());
			break;
		default:
			if (tmp >= '1' && tmp <= '9') {
				setFeld((y - 2) / 2, (x - 3) / 4, tmp - '0', 0);
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
