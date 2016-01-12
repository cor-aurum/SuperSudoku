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

#define CLEAR "cls"
#endif
void printFeld();
int setFeld(int x, int y, int eingabe, int lock);
int x = 3, y = 2;

/*
 * Gibt eine Nachricht unterhalb des Spielfeldes aus
 */
void meldungAusgeben(char* nachricht) {
	gotoxy(1, HOEHE * 2 + 2);
	printf("%s", nachricht);
	gotoxy(x, y);
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
			system(CLEAR);
			printFeld();
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
		default:
			if (tmp >= '1' && tmp <= '9') {
				setFeld((y - 2) / 2, (x - 3) / 5, tmp - '0', 0);
				system(CLEAR);
				printFeld();
				if (tmp == '8')
					meldungAusgeben("Es wurde eine Acht gedrückt");
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
