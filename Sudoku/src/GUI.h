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
void highlight(char c) {
	printf("\033[32m%c\033[0m", c);
}
#define CLEAR "clear"
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
void highlight(char c)
{
	CONSOLE_SCREEN_BUFFER_INFO csbiScreen;
	WORD wOldColAttr;
	GetConsoleScreenBufferInfo(STD_OUTPUT_HANDLE, &csbiScreen);
	wOldColAttr = csbiScreen.wAttributes;
	SetConsoleTextAttribute(STD_OUTPUT_HANDLE, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("%c",c);
	SetConsoleTextAttribute(STD_OUTPUT_HANDLE, wOldColAttr);
}

#define CLEAR "cls"
#endif
int setFeld(int x, int y, int eingabe, int lock);
int x = 3, y = 2;

/*
 * Gibt eine Nachricht unterhalb des Spielfeldes aus
 */
void meldungAusgeben(char* nachricht) {
	system(CLEAR);
	printFeld();
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
	int i, j;
	for (i = 0; i < HOEHE; i++) {
		for (j = 0; j < BREITE; j++) {
			printf("%s%s",
					(j % 3 == 0 && i != 0) ? "|" : (i % 3 != 0) ? "." : " ",
					(i % KACHELHOEHE == 0) ? "____" : "    ");
		}
		printf("%s\n", (i != 0) ? "|" : "");
		for (j = 0; j < BREITE; j++) {
			printf("%s ", (j % KACHELBREITE == 0) ? "|" : " ");
			if (!schutz[i][j]) {
				printf("%c  ", asFeld(feld[i][j]));
			} else {
				highlight(asFeld(feld[i][j]));
				printf("  ");
			}
		}
		printf("|     ");
		switch (i) {
		case 0:
			printf("wasd: Cursor bewegen");
			break;
		case 1:
			printf("Leerzeichen: Zahl löschen");
			break;
		case 2:
			printf("1-9: Zahl eintragen");
			break;
		case 3:
			printf("p: Spiel speichern");
			break;
		case 4:
			printf("o: Spiel laden");
			break;
		case 5:
			printf("l: Spiel lösen");
			break;
		case 6:
			printf("k: Spiel prüfen");
			break;
		case 8:
			printf("q: Programm beenden");
			break;
		}
		printf("\n");

	}
	for (j = 0; j < BREITE; j++) {
		printf("%s%s", (j % 3 == 0 && i != 0) ? "|" : " ",
				(i % KACHELHOEHE == 0) ? "____" : "    ");
	}
	printf("|\n");
}

/*
 * Empfaengt Tastatureingaben und steuert mit diesen das Spielfeld
 */
int eingabeLoop() {
	system(CLEAR);
	printFeld();
	gotoxy(x, y);
	while (1) {
		char tmp = getch();
		switch (tmp) {
		case 'w':
			if (y >= 4)
				y -= 2;
			break;
		case 'a':
			if (x >= 7)
				x -= 5;
			break;
		case 's':
			if (y < HOEHE * 2)
				y += 2;
			break;
		case 'd':
			if (x < BREITE * 5 - 2)
				x += 5;
			break;
		case 'q':
			system(CLEAR);
			exit(0);
			break;
		case 'p': {
			meldungAusgeben("Speicherort eingeben:");
			gotoxy(1, HOEHE * 2 + 3);
			char string[100];
			scanf("%99s", &string[0]);
			speichereFeld(string);
			meldungAusgeben("Gespeichert");
		}
			break;
		case 'o': {
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
			meldungAusgeben("Bitte warten, Sudoku wird gelöst");
			loeseSudokuMain();
			system(CLEAR);
			printFeld();
			break;
		case 'k':
			if (checkSudokuFormal()) {
				meldungAusgeben("Sudoku ist korrekt");
			} else {
				meldungAusgeben("Sudoku nicht ist korrekt");
			}
			break;
		case 'j':

			meldungAusgeben(getJoke());
			break;
		default:
			if (tmp >= '1' && tmp <= '9') {
				setFeld((y - 2) / 2, (x - 3) / 5, tmp - '0', 0);
				system(CLEAR);
				printFeld();
			}
			if (tmp == ' ') {
				setFeld((y - 2) / 2, (x - 3) / 5, 0, 0);
				system(CLEAR);
				printFeld();
			}
		}
		gotoxy(x, y);
	}
}
