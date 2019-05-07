/*****************************************************************************
 * SUDOKU ./Solver.h                                                         *
 * Implementierung eines Algorithmus zum Lösen eines vorgegebenen Sudokus    *
 *                                                                           *
 * Version 2 (2016-12-17) Felix Schütze dhbw@felix-schuetze.de               *
 *****************************************************************************/

/*
 * Rekursive Methode zum Lösen des Sudokus, wählt die erste gefundene Lösung aus.
 * @params:
 * **feld, das zu lösende Feld
 * x, die aktuelle x-Position
 * y, die aktuelle y-Position
 * start, die kleinste, bzw. größte Mögliche Einsetzbare Zahl
 * ende, die größte+1, bzw. kleinste-1 mögliche Einsetzbare Zahl
 */
int loese(int **feld, int x, int y, int start, int ende, int schritt) {
	int test;
	/*
	 * Prüft, ob ein Feld schon belegt ist und ruft die Methode in diesem Fall mit dem nächsten Feld auf
	 */
	if (feld[x][y]) {
		return (y + 1) < HOEHE ? loese(feld, x, y + 1, start, ende,schritt) :
				((x + 1) < BREITE) ? loese(feld, x + 1, 0, start, ende,schritt) : 1;
	} else {
		/*
		 * Probiert rekursiv alle Zahlen zwischen "start" und "ende" durch
		 */
		for (test = start; test != ende; test+=schritt) {
			if (!pruefePos(feld, x, y, test)) {
				feld[x][y] = test;
				/*
				 * Wenn das Ergebnis an dieser Stelle stimmen kann, so wird 1 zurückgegeben, wenn nicht, wird das Feld wieder gelöscht und die nächste Zahl wird probiert
				 */
				if (loese(feld, x, y, start, ende,schritt))
					return 1;
				else
					feld[x][y] = 0;

			}
		}
		/*
		 * Wenn keine einsetzbare Zahl gefunden wurde ist das Sudoku an dieser Stelle nicht lösbar. Es wird 0 zurückgegeben.
		 */
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
	int **feldh = allokieren(BREITE, HOEHE);
	int **feldr = allokieren(BREITE, HOEHE);

	int i, j;
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			feldh[i][j] = feld[i][j];
			feldr[i][j] = feld[i][j];
		}
	}

	// Aus zwei Richtungen lösen und erste Lösung behalten:
	loese(feldh, 0, 0, 1, MAX_ZAHL+1,1);
	loese(feldr, 0, 0, MAX_ZAHL+1, 0,-1);

	// Wenn die Lösungen identisch sind, ist die Lösung eindeutig...
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			/*
			 * ....wenn sie sich an nur einem Punkt unterscheiden nicht mehr
			 */
			if (feldh[i][j] != feldr[i][j] || feldh[i][j] == 0)
				return 0;
		}
	}
	for (i = 0; i < HOEHE; i++) {
		free(feldh[i]);
		free(feldr[i]);
	}
	free(feldh);
	free(feldr);
	return 1;
}

/*
 * loeseSudoku(feld)
 * Sucht nach einer Lösung fÃ¼r das durch "feld" gegebene Sudoku, indem "feld"
 * ausgefüllt wird.
 *
 * returns 0 - Wenn es keine Lösung gibt.
 * returns 1 - Wenn es eine Lösung gibt.
 */
int loeseSudoku(int **feld) {
	return loese(feld, 0, 0, 1, MAX_ZAHL + 1,1);
}
