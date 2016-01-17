/*****************************************************************************
 * SUDOKU ./solver.c                                                         *
 * Implementierung eines Algorithmus zum Lösen eines vorgegebenen Sudokus    *
 *                                                                           *
 * Version 2 (2016-12-17) Felix Schütze, Sascha Scherrer, Moritz Koch        *
 *****************************************************************************/

int gibHoeheOderBreite() {
	if (BREITE > HOEHE) {
		return BREITE;
	}
	return HOEHE;
}

int pruefe(int feld[BREITE][HOEHE], int breite, int hoehe, int zaehler) {
	int i;
	for (i = 0; i < gibHoeheOderBreite(); ++i) {
		if (feld[breite][i] == zaehler)
			return 0;
		if (feld[i][hoehe] == zaehler)
			return 0;
		if (feld[(breite / KACHELBREITE) * KACHELBREITE + (i % KACHELBREITE)][(hoehe
				/ KACHELHOEHE) * KACHELHOEHE + (i / KACHELHOEHE)] == zaehler)
			return 0;
	}
	return 1;
}

int loese(int feld[BREITE][HOEHE], int breite, int hoehe) {
	int test;
	if (breite < BREITE && hoehe < HOEHE) {
		if (feld[breite][hoehe] != 0) {
			if ((hoehe + 1) < HOEHE)
				return loese(feld, breite, hoehe + 1);
			else if ((breite + 1) < BREITE)
				return loese(feld, breite + 1, 0);
			else
				return 1;
		} else {
			for (test = 0; test < gibHoeheOderBreite(); ++test) {
				if (pruefe(feld, breite, hoehe, test + 1)) {
					feld[breite][hoehe] = test + 1;
					if ((hoehe + 1) < HOEHE) {
						if (loese(feld, breite, hoehe + 1))
							return 1;
						else
							feld[breite][hoehe] = 0;
					} else if ((breite + 1) < BREITE) {
						if (loese(feld, breite + 1, 0))
							return 1;
						else
							feld[breite][hoehe] = 0;
					} else
						return 1;
				}
			}
		}
		return 0;
	} else
		return 1;
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
	if (loese(feld, 0, 0)) {
		return 1;
	} else
		return 0;
}

