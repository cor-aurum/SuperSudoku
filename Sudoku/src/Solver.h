/*****************************************************************************
 * SUDOKU ./Solver.h                                                         *
 * Implementierung eines Algorithmus zum Lösen eines vorgegebenen Sudokus    *
 *                                                                           *
 * Version 2 (2016-12-17) Felix Schütze dhbw@felix-schuetze.de               *
 *****************************************************************************/



/*
 * Rekursive Methode zum Lösen des Sudokus, wählt die erste gefundene Lösung aus.
 */
int loese(int **feld, int x, int y) {
	int test;
	if (feld[x][y]) {
		return (y + 1) < HOEHE ? loese(feld, x, y + 1) :
				((x + 1) < BREITE) ? loese(feld, x + 1, 0) : 1;
	} else {
		for (test = 1; test <= MAX_ZAHL; test++) {
			if (!pruefePos(feld, x, y, test)) {
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
int loeseAbwaerts(int **feld, int x, int y) {
	int test;
	if (feld[x][y]) {
		return (y + 1) < HOEHE ? loeseAbwaerts(feld, x, y + 1) :
				((x + 1) < BREITE) ? loeseAbwaerts(feld, x + 1, 0) : 1;
	} else {
		for (test = MAX_ZAHL; test >= 1; test--) {
			if (!pruefePos(feld, x, y, test)) {
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
 * return 0 wenn das Sudoku nicht eindeutig gelöst werden kann.
 * return 1 wenn das Sudoku eindeutig lösbar ist.
 */
int eindeutig(int **feld) {
	// 2 Kopien des Feldes anlegen:
	int feldh[BREITE][HOEHE];
	int feldr[BREITE][HOEHE];
	int i, j;
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			feldh[i][j] = feld[i][j];
			feldr[i][j] = feld[i][j];
		}
	}

	// Aus zwei Richtungen lösen und erste Lösung behalten:
	loese(feldh, 0, 0);
	loeseAbwaerts(feldr, 0, 0);

	// Wenn die Lösungen identisch sind, ist die Lösung eindeutig
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			if (feldh[i][j] != feldr[i][j] || feldh[i][j]==0)
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
int loeseSudoku(int **feld) {
	return loese(feld, 0, 0);
}

