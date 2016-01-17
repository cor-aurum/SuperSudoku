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
#define LEGENDE 9
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
				if (!fehler[i][j]) {
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
		char *hilfe[LEGENDE] = { "n/m: Hilfe navigieren",
				"Pfeiltasten/wasd: Cursor bewegen", "Leerzeichen: Zahl löschen",
				"1-9: Zahl eintragen", "p: Spiel speichern", "o: Spiel laden",
				"l: Spiel lösen", "k: Spiel prüfen", "q: Programm beenden" };
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
 * Empfaengt Tastatureingaben und steuert mit diesen das Spielfeld
 */
int eingabeLoop() {
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
				meldungAusgeben("Laden der Datei fehlgeschlagen");
			} else {
				system(CLEAR);
				printFeld();
			}
		}
			break;
		case 'l':
			printFeld();
			meldungAusgeben("Bitte warten, Sudoku wird gelöst");
			loeseSudokuMain();
			system(CLEAR);
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
