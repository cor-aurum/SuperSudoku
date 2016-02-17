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
#include <termios.h> // TODO: Auf Win7 wurde kein Termios.h gefunden.
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

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("%c", c);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void highlightRot(char c) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	printf("%c", c);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
#define LEGENDE 15
int setFeld(int x, int y, int eingabe, int lock);
int legende = 0;
int zeichensatz = 0;
char *zeichen[3][20] = {
		{ "╔", "╟", "╠", "╬", "╫", "┼", "╪", "\u2564", "╦", "\u2562", "╣", "╗", "║", "│", "╚", "╧", "╩", "╝", "═══", "───" },
		{ " ", "|", "|", "+", "|", "+", "-", "-", "-", "|", "|", " ", "|", " ", " ", "-", "-", " ", "---", "   " },
		{ "#", "#", "#", "#", "#", "+", "#", "#", "#", "#", "#", "#", "#", "|", "#", "#", "#", "#", "###", "---" }
};

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
	switch (rand() % 8) {
	case 0:
		return "Welches Tier kann addieren? -- Ein Oktoplus.";
	case 1:
		return "Um Rekursion zu verstehen, muss man zunächst Rekursion verstehen.";
	case 2:
		return "Fragt ein Physiker einen Mathematiker:\nWas ist eigentlich ein 11-Dimensionaler Raum? Ich kann mir darunter nichts vorstellen.\nDer Mathematiker antwortet:\nGanz einfach, stell dir einen n-Dimensionalen Raum vor, und setze für n 11 ein. ";
	case 3:
		return "Die meisten Menschen haben überdurchschnittlich viele Beine!";
	case 4:
		return "Was hat Windows mit einem U-Boot gemeinsam? Kaum macht man ein Fenster auf, fangen die Probleme an.";
	case 5:
		return "Wieviele Programmierer braucht man, um eine Glühbirne zu wechseln? Keinen, das ist ein Hardware-Problem!";
	case 6:
		return "42 ist die größte Primzahl mit mehr als zwei Teilern.";
	default:
		return "Wie nennt man ein Rind mit Root-Rechten? Sudo ku";
	}
}

/*
 * Diese Methode gibt einen Wert im Sudoku als Druckbares Zeichen zurück.
 */
char asFeld(int c) {
	if (c == 0)
		return ' ';
	if (c > 9)
		return (char)(c - 10 + 'A');
	return (char)(c + 48);
}

/*
 * Diese Methode gibt ein Feuerwerk in der Konsole aus
 */
void printFeuerwerk() {
	system(CLEAR);
	printf("                                               ####         \n");
	printf("    #                          #         ####### #######    \n");
	printf("  ######         ##          ######      #####     #####    \n");
	printf(" #######        ####         ######       ##         ##     \n");
	printf("  #####     #####  #####      ####       #######  ######    \n");
	printf("  ## ##      ##      ##       #  #            ######        \n");
	printf("       ##    ##      ##           ##            ##          \n");
	printf("       ###  #####  #####          ###                       \n");
	printf("      ####      #####            ####                       \n");
	printf("    ###  ###     ##            ########                     \n");
	printf("######    #######          #################                \n");
	printf("######    ########         #################         ###    \n");
	printf("    ###  ###                    #######         ############\n");
	printf("      ####                       ####           #####   ####\n");
	printf("       ###          ##            ###            ##       # \n");
	printf("        #         ######          ##            #####   ####\n");
	printf("             ##    ####                         ############\n");
	printf("            ####   ####                              ##     \n");
	printf("        ##### #####                           #             \n");
	printf("         ##      ##             ##         # ### #          \n");
	printf("         ##     ##            #####        #######          \n");
	printf("        ####    ###       ######  ######    #####           \n");
	printf("        ##### #####       ##         ##     #####           \n");
	printf("            ####           ##       ##                      \n");
	printf("             ##           ###        ##                     \n");
	printf("                          ######  ######                    \n");
	printf("             ##               #####               ##        \n");
	printf("         ##########             ##                ##        \n");
	printf("          ########                             ########     \n");
	printf("           ######              #######          ######      \n");
	printf("                     ##       ########          ######      \n");
	printf("                    ####        #####                       \n");
	printf("                    ####                                    \n");
	printf("              ################                              \n");
	printf("              ################                              \n");
	printf("                    ####                                    \n");
	printf("                    ####                                    \n");
	printf("                     ##                                     \n");
	printf("g für ein neues Sudoku                                      \n");
}

/*
 * Diese Methode schreibt in den standartout eine gut lesbare Darstellung des Sudokus
 */
void printFeld() {
	system(CLEAR);
	if (getSpalten() < BREITE * 4 + 1) {
		fprintf(stderr, "Das Konsolenfenster ist schmaler als das Sudokufeld. \n"
				"Daher kann das Feld nicht angezeigt werden. \n"
				"Wählen sie ein größeres Konsolenfenster oder eine kleinere Kachelgröße.\n");
		exit(EXIT_FAILURE);
	}
	int i, j;
	for (i = 0; i < HOEHE; i++) {
		for (j = 0; j < BREITE; j++) {
			printf("%s%s",
					j == 0 ?
							i == 0 ? zeichen[zeichensatz][0] :
							i % KACHELHOEHE != 0 ?
									zeichen[zeichensatz][1] : zeichen[zeichensatz][2]
					:
					(j % KACHELBREITE == 0 && i != 0) ?
							i % KACHELHOEHE == 0 ?
									zeichen[zeichensatz][3] : zeichen[zeichensatz][4]
					:
					(i % KACHELHOEHE != 0) ? zeichen[zeichensatz][5] :
					i != 0 ? zeichen[zeichensatz][6] :
					j % KACHELBREITE != 0 ?
							zeichen[zeichensatz][7] : zeichen[zeichensatz][8],
					(i % KACHELHOEHE == 0) ?
							zeichen[zeichensatz][18] : zeichen[zeichensatz][19]);
		}
		printf("%s\n",
				(i != 0) ?
						i % KACHELHOEHE != 0 ?
								zeichen[zeichensatz][9] : zeichen[zeichensatz][10]
						:zeichen[zeichensatz][11]);
		for (j = 0; j < BREITE; j++) {
			printf("%s ",
					(j % KACHELBREITE == 0) ?
							zeichen[zeichensatz][12] : zeichen[zeichensatz][13]);
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
		printf("%s", zeichen[zeichensatz][12]);
		printf("    ");
		char *hilfe[LEGENDE] = { "Pfeiltasten/wasd: Cursor bewegen", "DUMMY",
				"Leerzeichen: Zahl löschen", "l: Spiel lösen", "p: Spiel speichern",
				"o: Spiel laden", "c: Spiel leeren", "h: Nächste Hilfeseite",
				"e: Sudoku auf Eindeutigkeit prüfen", "t: Zeichentabelle aufrufen",
				"y: Schreibschutz setzen", "x: Schreibschutz aufheben",
				"g: Sudoku generieren", "u: Über", "q: Programm beenden" };
		int nummer = legende * HOEHE + i;
		if (nummer < LEGENDE && getSpalten() > BREITE * 4 + 40) {
			if (nummer != 1) {
				printf("%s", hilfe[nummer]);
			} else {
				printf("1-%c: Zahl eintragen",
						MAX_ZAHL < 10 ? MAX_ZAHL + '0' : MAX_ZAHL - 10 + 'A');
			}
		}
		printf("\n");

	}
	for (j = 0; j < BREITE; j++) {
		printf("%s%s",
				j == 0 ? zeichen[zeichensatz][14] :
				j % KACHELBREITE != 0 ?
						zeichen[zeichensatz][15] : zeichen[zeichensatz][16],
				(i % KACHELHOEHE == 0) ? zeichen[zeichensatz][18] : "   ");
	}
	printf("%s", zeichen[zeichensatz][17]);
	printf("\n");
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
		if (y)
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
		printf("(  ____ \\|\\     /|(  __  \\ (  ___  )| \\    /\\|\\     /|\n");
		printf("| (    \\/| )   ( || (  \\  )| (   ) ||  \\  / /| )   ( |\n");
		printf("| (_____ | |   | || |   ) || |   | ||  (_/ / | |   | |\n");
		printf("(_____  )| |   | || |   | || |   | ||   _ (  | |   | |\n");
		printf("      ) || |   | || |   ) || |   | ||  ( \\ \\ | |   | |\n");
		printf("/\\____) || (___) || (__/  )| (___) ||  /  \\ \\| (___) |\n");
		printf("\\_______)(_______)(______/ (_______)|_/    \\/(_______)\n\n\n");
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
		char tmp = (char) getch();
		switch (tmp) {
		case 'w': // Kästchen hoch
		case UP:
			if (y >= 3)
				y -= 2;
			break;
		case 'a': // Kästchen nach links
		case LEFT:
			if (x >= 4)
				x -= 4;
			break;
		case 's': // Kästchen nach unten
		case DOWN:
			if (y < HOEHE * 2
#if defined(Win32)
					-2
#endif
					)
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
			meldungAusgeben("Speichern: Speicherort eingeben:");
			gotoxy(1, HOEHE * 2 + 3);
			char pfad[100];
			scanf("%99s", &pfad[0]);
			speichereFeld(pfad, 1);
			printFeld();
			meldungAusgeben("Gespeichert");
		}
			break;
		case 't': { // Zeichentabelle an aktueller Posiiton anzeigen
			//printFeld();
			//fflush(stdout);
			int i;
			printf("%s", zeichen[zeichensatz][0]);
			for (i = 0; i < MAX_ZAHL; i++) {
				printf("%s", zeichen[zeichensatz][18]);
			}
			printf("%s", zeichen[zeichensatz][11]);
			gotoxy(x, y + 1);
			printf("%s", zeichen[zeichensatz][12]);
			for (i = 0; i < MAX_ZAHL; i++) {
				printf(" %c ", asFeld(i + 1));
			}
			printf("%s", zeichen[zeichensatz][12]);
			gotoxy(x, y + 2);
			printf("%s", zeichen[zeichensatz][14]);
			for (i = 0; i < MAX_ZAHL; i++) {
				printf("%s", zeichen[zeichensatz][18]);
			}
			printf("%s", zeichen[zeichensatz][17]);
			gotoxy(x + 2, y + 1);
			int pos = 0, weiter = 1;
			while (weiter) {
				char tmp2 = (char) getch();
				switch (tmp2) {
				case LEFT:
					if (pos > 0) {
						pos--;
						gotoxy(x + 2 + pos * 3, y + 1);
					}
					break;
				case RIGHT:
					if (pos < MAX_ZAHL - 1) {
						pos++;
						gotoxy(x + 2 + pos * 3, y + 1);
					}
					break;
				case ' ':
#if !defined(WIN32)
					setFeld((y - 2) / 2, (x - 3) / 4, pos + 1, 0);
#else
					setFeld((y - 1) / 2, (x - 2) / 4, pos+1, 0);
#endif
					testSudokuFormal(feld);
					weiter = 0;
					break;
				}
			}
			if (!pruefeVollstaendig(feld))
				printFeld();
			else
				printFeuerwerk();
		}
			break;
		case 'o': { // Öffnen
			printFeld();
			meldungAusgeben("Öffnen: Speicherort eingeben:");
			gotoxy(1, HOEHE * 2 + 3);
			char string[100];
			scanf("%99s", &string[0]);
			int leseDateiReturn = leseDatei(string);
			if (leseDateiReturn < 0) {
				printFeld();
				meldungAusgeben("Laden der Datei fehlgeschlagen");
			} else {
				system(CLEAR);
				printFeld();
				char err[150];
				snprintf(err, 150,
						"Datei geladen: %s\nEs wurden %d Fehler gefunden und korrigiert.",
						string, leseDateiReturn);
				meldungAusgeben(err);
			}
		}
			break;
		case 'l':
			// Sudoku lösen
			printFeld();
			if (!loeseSudoku(feld)) {
				meldungAusgeben("Sudoku ist nicht lösbar");
			} else {
				printFeld();
			}
			break;
		case 'g':
			// Sudoku generieren
		{
			printFeld();
			meldungAusgeben("Schwierigkeitsstufe (1-9) angeben:");
			fflush(stdout);
			int t;
			do {
				t = getch();
			} while (t < '1' || t > '9');
			printFeld();
			meldungAusgeben("Sudoku wird generiert...");
			fflush(stdout);
			generiereSudoku(feld, t - '0');
			printFeld();
			break;
		}
		case 'e': // Eindeutigkeit prüfen
			printFeld();
			if (!testSudokuFormal(feld)) {
				if (eindeutig(feld)) {
					meldungAusgeben("Sudoku ist eindeutig");
				} else {
					meldungAusgeben("Sudoku ist nicht eindeutig");
				}
			} else {
				printFeld();
				meldungAusgeben("Sudoku ist nicht korrekt");
			}
			break;
		case 'h':
			// Anzeige der Befehlsoptionen: Zurück
			if (legende < (LEGENDE / HOEHE))
				legende++;
			else
				legende = 0;
			printFeld();
			break;
		case 'c':
			// Sudoku leeren (Setzt Feld und Schutz zurück)
			for (i = 0; i < BREITE; i++) {
				for (j = 0; j < HOEHE; j++) {
					feld[i][j] = 0;
					schutz[i][j] = 0;
				}
			}
			printFeld();
			break;
		case 'x':
			// Schutz aufheben
			for (i = 0; i < BREITE; i++) {
				for (j = 0; j < HOEHE; j++) {
					/* schutz > 0 ist lock, schutz < 0 ist fehler */
					if (schutz[i][j] > 0)
						schutz[i][j] = 0;
				}
			}
			printFeld();
			break;
		case 'y':
			// Aktuelle Felder schützen
			for (i = 0; i < BREITE; i++)
				for (j = 0; j < HOEHE; j++)
					/* schutz > 0 ist lock, schutz < 0 ist fehler */
					if (!(schutz[i][j] < 0))
						schutz[i][j] = feld[i][j];
			printFeld();
			break;
		case 'u':
			// Über: Zeigt eine Info über das Programm an
			printUber(1);
			break;
		case 'j':
			// Macht nichts besonderese :)
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

				testSudokuFormal(feld);
				if (!pruefeVollstaendig(feld))
					printFeld();
				else
					printFeuerwerk();
			}
			if (tmp == ' ' || tmp == '0') {
#if !defined(WIN32)
				setFeld((y - 2) / 2, (x - 3) / 4, 0, 0);
#else
				setFeld((y - 1) / 2, (x - 2) / 4, 0, 0);
#endif
				system(CLEAR);
				testSudokuFormal(feld);
				printFeld();
			}
		}
		gotoxy(x, y);
	}
}
