
/*
 * Prüft für die durch x und y gegebene Position, ob der wert hier eingesetzt werden darf
 * returns -1 - Wert liegt nicht im zugelassenen Bereich
 * returns  0 - Wert darf eingesetzt werden
 * returns  n - Wert anzahl an Fehlern, die ein einsetzen des Wertes verursachen würde.
 */
int pruefePos(int **feld, int x, int y, int wert) {
	int fehler = 0, aktX, aktY;

	if (wert == 0)
		return 0; // 0 darf immer eingesetzt werden.

	if (wert > MAX_ZAHL)
		return -1; // Der Wert liegt nicht imzugelassenen Bereich

	// In Zeile
	for (aktX = 0; aktX < BREITE; aktX++)
		if (feld[aktX][y] == wert && aktX != x) {
			fehler++;
		}
	// In Spalte
	for (aktY = 0; aktY < BREITE; aktY++)
		if (feld[x][aktY] == wert && aktY != y) {
			fehler++;
		}

	// In Kachel
	for (aktX = (x / KACHELBREITE) * KACHELBREITE;
			aktX < (x / KACHELBREITE + 1) * KACHELBREITE; aktX++)
		for (aktY = (y / KACHELHOEHE) * KACHELHOEHE;
				aktY < (y / KACHELHOEHE + 1) * KACHELHOEHE; aktY++)
			if (feld[aktX][aktY] == wert && aktX != x && aktY != y) {
				fehler++;
			}

	return fehler;
}

/*
 * Gibt wahr zurück, wenn alle Felder des Sudokus korrekt ausgefüllt sind
 */
int pruefeVollstaendig(int **feld)
{
	int i,j;
	if(testSudokuFormal(feld))
		return 0;
	for(i=0;i<BREITE;i++)
	{
		for(j=0;j<HOEHE;j++)
		{
			if(!feld[i][j])
				return 0;
		}
	}
	return 1;
}

int fehlerMarkieren(int **feld, int **schutz, int x,
		int y, int ignSchutz) {
	int aktX = 0, aktY = 0, wert = feld[x][y], markiert = 0;

	// Fehler in gleicher Zeile markieren
	for (aktX = 0; aktX < BREITE; aktX++) {
		if (feld[aktX][y] == wert && (ignSchutz || schutz[aktX][y] <= 0)) {
			schutz[aktX][y] = -wert;
			markiert++;
		}
	}

	// Fehler in gleicher Spalte markeiren
	for (aktY = 0; aktY < HOEHE; aktY++) {
		if (feld[x][aktY] == wert && (ignSchutz || schutz[x][aktY] <= 0)) {
			schutz[x][aktY] = -wert;
			markiert++;
		}
	}

	// Fehler in gleicher Kachel markieren
	for (aktX = (x / KACHELBREITE) * KACHELBREITE;
			aktX < (x / KACHELBREITE + 1) * KACHELBREITE; aktX++)
		for (aktY = (y / KACHELHOEHE) * KACHELHOEHE;
				aktY < (y / KACHELHOEHE + 1) * KACHELHOEHE; aktY++)
			if (feld[aktX][aktY] == wert
					&& (ignSchutz || schutz[aktX][aktY] <= 0)) {
				schutz[aktX][aktY] = -wert;
				markiert++;
			}

	return markiert;
}


/*
 * testSudokuFormal(int feld[][])
 * Prüft das Sudoku auf Formale korrektheit und setzt Fehlermarkierungen in schutz.
 * returns 0 - wenn das feld formal korrekt ist
 * returns n - die Anzahl der Fehler, die gefunden wurden.
 */
int testSudokuFormal(int **feld) {
	int x = 0, y = 0, f = 0;

	// Fehlerspeicher zurücksetzen (Schutz > 0 ist gelockt, Schutz < 0 ist Fehler)
	for (y = 0; y < HOEHE; y++) {
		for (x = 0; x < BREITE; x++) {
			if (schutz[x][y] < 0)
				schutz[x][y] = 0;
		}
	}

	// Alle gelockten Felder prüfen:
	for (x = 0; x < BREITE; x++) {
		for (y = 0; y < HOEHE; y++) {
			if (schutz[x][y] > 0 && feld[x][y] > 0
					&& pruefePos(feld, x, y, feld[x][y])) {
				// Fehler gefunden:
				// Alle anderen Vorkommen in Reihe, Spalte und Kachel markieren
				f++;

				// Fehler markieren (Schreibschutz wird beachtet)
				if(fehlerMarkieren(feld, schutz, x, y, 0) == 0){
					// Es gab einen Fehler in den gelockten Feldern, also wird
					// jetzt der Schreibschutz ignoriert:
					fehlerMarkieren(feld, schutz, x, y, 1);
				}
			}
		}
	}

	// Alle nicht gelockten Felder prüfen
	// (jetzt sind auf jeden Fall die gelockten Felder alle korrekt):
	for (x = 0; x < BREITE; x++) {
		for (y = 0; y < HOEHE; y++) {
			if (schutz[x][y] <= 0 && feld[x][y] > 0
					&& pruefePos(feld, x, y, feld[x][y])) {
				// Fehler gefunden
				f++;
				fehlerMarkieren(feld, schutz, x, y, 0);
			}
		}
	}

	return f;
}

