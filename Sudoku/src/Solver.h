/*****************************************************************************
 * SUDOKU ./solver.c                                                         *
 * Implementierung eines Algorithmus zum Lösen eines vorgegebenen Sudokus    *
 * Nicht getestet.                                                           *
 * Version 1 (2015-12-29) Sascha Scherrer <dhbw.scherrer@gmail.com>          *
 *****************************************************************************/

/*
 * feldOptionen(feld, kannEnthalten, posX, posY)
 * Ermittelt die Ausfülloptionen für das durch posX und posY 
 * gegebene Feld im Sudouku.
 * Speichert die Möglichkeiten im Feld kannEnthalten[x][y][wert].
 * Dabei sind x und y die Feldkoordinaten posX und posY und wert
 * der Wert, der vorkommen darf (0-> darf nicht, 1->darf vorkommen)
 * In kannEnthalten[x][y][0] wird die Anzahl der möglichen
 * Werte notiert, die eingesetzt werden dürfen.
 *
 * returns n - Anzahl der Möglichkeiten zum Ausfüllen des Feldes.
 * returns 0 - Wenn es keine Möglichkeiten zum Einsetzen gibt, das
 *             bedeutet, dass das Sudoku nicht lösbar ist.
 * returns -1 - Wenn das Feld unveränderlich ist (da es in
 *              in der Eingabe bereits vorgegeben ist)
 */
int feldOptionen(int feld[BREITE][HOEHE],
		int kannEnthalten[BREITE][HOEHE][MAX_ZAHL + 1], int posX, int posY) {
	int aktWert = 0, aktFeld = 0, aktX = posX, aktY = posY,
			akzeptierteWerte = 0;

//	printf("\n     posX = %d,   posY = %d \n", posX, posY);

	if (!feld[posX][posY]) {
		/* Feld darf erstmal jeden Wert annehmen */
		for (aktWert = 1; aktWert <= MAX_ZAHL; aktWert++)
			kannEnthalten[posX][posY][aktWert] = 1;

		/* Alle Werte, die schon in der Zeile stehen, dürfen nicht angenommen werden */
		for (aktX = 0; aktX < BREITE; aktX++)
			if (feld[aktX][posY] >= 1 && feld[aktX][posY] <= MAX_ZAHL) {
				kannEnthalten[posX][posY][feld[aktX][posY]] = 0;
//				printf("feldOptionen: Zeilentest: (%d|%d) = %d (wird gestrichen)\n",
//						aktX, posY, feld[aktX][posY]);
			}

		/* Alle Werte, die schon in der Spalte stehen, dürfen nicht angenommen werden */
		for (aktY = 0; aktY < HOEHE; aktY++)
			if (feld[posX][aktY] >= 1 && feld[posX][aktY] <= MAX_ZAHL) {
				kannEnthalten[posX][posY][feld[posX][aktY]] = 0;
//				printf("feldOptionen: Spaltentest: (%d|%d) = %d (wird gestrichen)\n",
//					posX, aktY, feld[posX][aktY]);
			}

		/* Alle Werte, die schon in der Kachel stehen, dürfen nicht angenommen werden */
		int kX = posX;
		int kY = posY;
		while (kX % KACHELBREITE)
			kX--;
		while (kY % KACHELHOEHE)
			kY--;

		for (aktFeld = 0; aktFeld < KACHELBREITE * KACHELHOEHE; aktFeld++) {
			aktX = aktFeld % KACHELBREITE + kX;
			aktY = aktFeld / KACHELHOEHE + kY;

			if (feld[aktX][aktY] >= 1 && feld[aktX][aktY] <= MAX_ZAHL) {
				kannEnthalten[posX][posY][feld[aktX][aktY]] = 0;
//				printf("feldOptionen: Kacheltest: (%d|%d) = %d (wird gestrichen)\n",
//						aktX, aktY, feld[aktX][aktY]);
			}
		}

		/* Wie viele Werte sind für dieses Feld möglich */
		for (aktWert = 1; aktWert <= MAX_ZAHL; aktWert++)
			if (kannEnthalten[posX][posY][aktWert])
				akzeptierteWerte++;

		/* Gib das ganze zur Kontrolle mal aus: */
//		printf("feldOptionen: Feld (%d|%d) in Kachel (%d|%d) kann %d Werte enthalten: ",
//			 posX, posY, kX, kY, akzeptierteWerte);
//		for(aktWert = 1; aktWert <= MAX_ZAHL; aktWert++)
//			if(kannEnthalten[posX][posY][aktWert])
//				printf("%d ", aktWert);
//		printf("\n");
		kannEnthalten[posX][posY][0] = akzeptierteWerte;
		return akzeptierteWerte;
	} else {
//		printf("feldOptionen: Feld (%d|%d) ist unveränderbar.\n", posX, posY);
		kannEnthalten[posX][posY][0] = 1;
		return -1;
	}
}

/*
 * testeGeloest(feld)
 * Testet, ob für alle Felder des angegebenen Feldes der Wert
 * von Null verschieden ist. Wenn es kein Feld mit dem Wert
 * Null gibt, ist das Sudoku gelöst, andernfalls ist es nicht
 * gelöst.
 *
 * returns 1 - Sudoku ist gelöst.
 * returns 0 - Sudoku ist nicht gelöst.
 */
int testeGeloest(int feld[BREITE][HOEHE]) {
	int i;
	for (i = 0; i < BREITE * HOEHE; i++)
		if (feld[i % BREITE][i / HOEHE])
			return 0;
	return 1;
}

/*
 * findeGleicheMoeglichkeiten(feld, kannEnthalten, gleicheMoeglichkeiten)
 * Findet Felder in einer Zeile, Spalte oder Kachel, bei denen die gleichen
 * Möglichkeiten zum Einsetzen einer Zahl gegeben sind.
 */
int findeGleicheMoeglichkeiten(int feld[BREITE][HOEHE],
		int kannEnthalten[BREITE][HOEHE][MAX_ZAHL + 1]) {
	// gleichesPaarFeld[paarId][feldNr][0/1] = koordinate;
	// gleichesPaarElemente[paarId][wertNr] = wert;
	return 0;
}

/*
 * löst offensichtliche Zahlen im Sudoku. Gibt 0 bei einem Fehler zurück
 */
int loeseSimple(int feld[BREITE][HOEHE]) {
	int kannEnthalten[BREITE][HOEHE][MAX_ZAHL + 1], x = 0, y = 0, z = 0,
			weitermachen = 0;
	do {
		weitermachen = 0;
		for (x = 0; x < BREITE; x++)
			for (y = 0; y < HOEHE; y++) {
				/* Alle Felder durchgehen: Wie viele Lösungen gibt es? */
				int tmp = feldOptionen(feld, kannEnthalten, x, y);
				if (tmp == 0)
					return 0;
				if (tmp == 1) {
					/* Für das Feld gibt nur eine Möglichkeit: *
					 * Lösung für das Feld ins Feld einsetzen  */
					for (z = 1; z <= MAX_ZAHL; z++)
						if (kannEnthalten[x][y][z])
							feld[x][y] = z;
					/* Felddurchgang zu Ende */
					weitermachen++;
				}
			}
	} while (weitermachen);
	return 1;
}

void backtracking(int feld[BREITE][HOEHE],
		int kannEnthalten[BREITE][HOEHE][MAX_ZAHL + 1]) {
	int x = 0, y = 0, z = 0;
	for (x = 0; x < BREITE; x++)
		for (y = 0; y < HOEHE; y++) {
			/* Alle Felder durchgehen: Wie viele Lösungen gibt es? */
			int tmp = feldOptionen(feld, kannEnthalten, x, y);
			if (tmp >= 1) {
				for (z = 1; z <= MAX_ZAHL; z++) {
					if (kannEnthalten[x][y][z])
						feld[x][y] = z;
					if (!loeseSimple(feld)) {
						kannEnthalten[x][y][z] = 0;
						feld[x][y] = 0;
					}
				}

			}
		}
}

/*
 * loeseSudoku(feld)
 * Sucht nach einer Lösung für das durch "feld" gegebene Sudoku, indem "feld"
 * ausgefüllt wird.
 *
 * returns 0 - Wenn es keine Lösung gibt.
 * returns 1 - Wenn es genau eine Lösung gibt.
 * returns n - Wenn es n Lösungen gibt.
 */
// Nach J. F. Crook, siehe http://www.ams.org/journals/notices/200904/rtx090400460p.pdf
int loeseSudoku(int feld[BREITE][HOEHE]) {
	if (testSudokuFormal(feld))
		return 0;
	//meldungAusgeben("Bitte warten, Sudoku wird gelöst");

	int kannEnthalten[BREITE][HOEHE][MAX_ZAHL + 1], copyfeld[BREITE][HOEHE], i,
			j;
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			copyfeld[i][j] = feld[i][j];
		}
	}
	/*
	 * Schritt 1: Eindeutige Ersetzungen vornehmen.
	 * Eindeutig ist eine ersetzung, wenn es für ein Feld genau einen 
	 * Kandidaten gibt, der eingesetzt werden kann. Hier wird für jedes
	 * Feld geprüft, wie viele Kandidaten es gibt. Gibt es nur einen
	 * Kandidaten, wir dieser eingesetzt. Wurde eine ersetzung vorgenommen,
	 * werden nach dem letzten Feld (Ecke unten rechts) erneut alle Felder
	 * durchgegange, da es jetzt weitere eindeutig ersetzbare Felder geben
	 * kann.
	 * Gibt es ein nicht vorgegebenes Feld, für das keine Zahl eingesetzt
	 * werden kann ohne die Sudoku-Regeln zu verletzen, so ist das Sudoku
	 * nicht lösbar.
	 * Wenn das Sudoku nach diesem Schritt noch ungelöst ist und noch nicht
	 * festgestellt wurde, dass es nicht lösbar ist, dann es lösbar, aber
	 * nicht eindeutig lösbar.
	 */
	if (!loeseSimple(feld))
		return 0;

	/*
	 * Schritt 2: Prüfen, ob das Sudoku jetzt gelöst ist.
	 * Hier wird geprüft, ob das Sudoku bereits gelöst werden konnte. Ist dies
	 * der Fall, sind wir fertig. Wenn es keine Lösung gibt, müssen wir auf 
	 * etwas umständlicherem Weg zur Lösung kommen, siehe Schritt 3.
	 * Wenn es keine Lösung gibt, ist zu diesem Zeitpunkt bereits 0 von der
	 * Funktion zurückgegeben worden.
	 */

	if (testeGeloest(feld))
		return 1; /* Sudoku ist gelöst, andernfalls weitermachen (Schritt 3) */

	/*
	 * Schritt 3: Eine Lösung finden und ermitteln, wie viele Lösungen es gibt.
	 */

	backtracking(copyfeld, kannEnthalten);

	return -1;

}
