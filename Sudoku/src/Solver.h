/*****************************************************************************
 * SUDOKU ./solver.c                                                         *
 * Implementierung eines Algorithmus zum Lösen eines vorgegebenen Sudokus    *
 *                                                                           *
 * Version 2 (2016-12-17) Felix Schütze, Sascha Scherrer, Moritz Koch        *
 *****************************************************************************/

/*
 * Prüft, ob zaehler an Punkt x/y eingesetzt werden kann
 */
int pruefe(int feld[BREITE][HOEHE], int x, int y, int zaehler) {
	int i;
	for (i = 0; i < MAX_ZAHL; ++i) {
		if (feld[x][i] == zaehler || feld[i][y] == zaehler
				|| feld[(x / KACHELBREITE) * KACHELBREITE + (i % KACHELBREITE)][(y
						/ KACHELHOEHE) * KACHELHOEHE + (i / KACHELHOEHE)]
						== zaehler)
			return 0;
	}
	return 1;
}

int loese(int feld[BREITE][HOEHE], int x, int y) {
	int test;
	if (feld[x][y] != 0) {
		return (y + 1) < HOEHE ? loese(feld, x, y + 1) :
				((x + 1) < BREITE) ? loese(feld, x + 1, 0) : 1;
	} else {
		for (test = 0; test < MAX_ZAHL; ++test) {
			if (pruefe(feld, x, y, test + 1)) {
				feld[x][y] = test + 1;
				if ((y + 1) < HOEHE) {
					if (loese(feld, x, y + 1))
						return 1;
					else
						feld[x][y] = 0;
				} else if ((x + 1) < BREITE) {
					if (loese(feld, x + 1, 0))
						return 1;
					else
						feld[x][y] = 0;
				} else
					return 1;
			}
		}
	}
	return 0;
}

/*
 * loeseSudoku(feld)
 * Sucht nach einer Lösung für das durch "feld" gegebene Sudoku, indem "feld"
 * ausgefüllt wird.
 *
 * returns 0 - Wenn es keine Lösung gibt.
 * returns 1 - Wenn es eine Lösung gibt.
 */
// Nach J. F. Crook, siehe http://www.ams.org/journals/notices/200904/rtx090400460p.pdf
int loeseSudoku(int feld[BREITE][HOEHE]) {
	return loese(feld, 0, 0);
}

