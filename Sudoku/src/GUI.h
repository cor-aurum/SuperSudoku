/******************************************************************************
 * SUDOKU ./reader.c                                                          *
 * Steuerung für die Händische Manipulation von Sudokus.
 * @author Felix Schütze, dhbw@felix-schuetze.de
 ******************************************************************************/
#include <termios.h>
void printFeld();
int setFeld(int x, int y, int eingabe, int lock);

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

void gotoxy(int x, int y) {
	printf("%c[%d;%df", 0x1B, y, x);
}

int eingabeLoop() {
	int x = 3, y = 2;
	system("clear");
	printFeld();
	gotoxy(x, y);
	while (1) {
		char tmp = getch();
		switch (tmp) {
		case 'w':
			if (y >= 4)
				y-=2;
			break;
		case 'a':
			if (x >= 7)
				x-=5;
			break;
		case 's':
			if (y < HOEHE*2)
			y+=2;
			break;
		case 'd':
			if (x < BREITE*5-2)
			x+=5;
			break;
		default:
			if (tmp >= '1' && tmp <= '9') {
				setFeld((y-2)/2,(x-3)/5,tmp-'0',0);
				system("clear");
				printFeld();
				//fflush(stdout);
			}
		}
		gotoxy(x, y);
	}
}
