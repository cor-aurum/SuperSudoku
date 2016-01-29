/*****************************************************************************
 * SUDOKU ./Solver.h                                                         *
 * Implementierung eines Algorithmus zum Lösen eines vorgegebenen Sudokus    *
 *                                                                           *
 * Version 2 (2016-12-17) Felix Schütze dhbw@felix-schuetze.de               *
 *****************************************************************************/

/*
 * Prüft, ob wert an Punkt x/y eingesetzt werden kann
 * returns 1 - Wert darf eingesetzt werden
 * returns 0 - Wert ist hier nicht zulässig
 */
int pruefe(int feld[BREITE][HOEHE], int x, int y, int wert) {
	int i;

	// Gegen Sudoku-Regeln verstoßen ist nicht erlaubt:
	// TODO: MAX_ZAHL ist hier fehl am Platz! Wenn HOEHE oder BREITE ungleich MAX_ZAHL sind, bekommen wir Probleme mit feld[x][y]!
	// Siehe Reader.h pruefePos als alternative.
	for (i = 0; i < MAX_ZAHL; i++) {
		if (feld[x][i] == wert || feld[i][y] == wert
				|| feld[(x / KACHELBREITE) * KACHELBREITE + (i % KACHELBREITE)][(y
						/ KACHELHOEHE) * KACHELHOEHE + (i / KACHELHOEHE)]
						== wert)
			return 0;
	}

	// Wenn kein Verstoß vorliegt, ist der Wert zulässig:
	return 1;
}

/*
 * Rekursive Methode zum Lösen des Sudokus, wählt die erste gefundene Lösung aus.
 * TODO: Probleme treten auf, wenn das Sudoku nicht lösbar ist.
 */
int loese(int feld[BREITE][HOEHE], int x, int y) {
	int test;
	if (feld[x][y]) {
		return (y + 1) < HOEHE ? loese(feld, x, y + 1) :
				((x + 1) < BREITE) ? loese(feld, x + 1, 0) : 1;
	} else {
		for (test = 1; test <= MAX_ZAHL; test++) {
			if (pruefe(feld, x, y, test)) {
				feld[x][y] = test;

				if (loese(feld, x, y))
					return 1;
				else
					feld[x][y] = 0;

			}
		}
		return 0;
	}
}

/*
 * Identisch zu "loese", zählt aber ab- statt aufwärts
 */
int loeseAbwaerts(int feld[BREITE][HOEHE], int x, int y) {
	int test;
	if (feld[x][y]) {
		return (y + 1) < HOEHE ? loeseAbwaerts(feld, x, y + 1) :
				((x + 1) < BREITE) ? loeseAbwaerts(feld, x + 1, 0) : 1;
	} else {
		for (test = MAX_ZAHL; test >= 1; test--) {
			if (pruefe(feld, x, y, test)) {
				feld[x][y] = test;

				if (loeseAbwaerts(feld, x, y))
					return 1;
				else
					feld[x][y] = 0;

			}
		}
		return 0;
	}
}

/*
 * Mehode die ein Sudoku auf Eindeutigkeit testet.
 * Gibt das Ergebnis zurück
 */
int eindeutig(int feld[BREITE][HOEHE]) {
	int feldh[BREITE][HOEHE];
	int feldr[BREITE][HOEHE];
	int i, j;
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			feldh[i][j] = feld[i][j];
			feldr[i][j] = feld[i][j];
		}
	}
	loese(feldh, 0, 0);
	loeseAbwaerts(feldr, 0, 0);
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			if (feldh[i][j] != feldr[i][j])
				return 0;
		}
	}
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
int loeseSudoku(int feld[BREITE][HOEHE]) {
	return loeseAbwaerts(feld, 0, 0);
}

