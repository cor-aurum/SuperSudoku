/*
 * Diese Methode schreibt ein im RAM gespeichertes Sudoku in die Datei datei[].
 */
int speichereFeld(char datei[], int y) {
	FILE *fp;

	fp = fopen(datei, "w");

	if (fp == NULL) {
		fprintf(stderr, "Datei %s konnte nicht geschrieben werden.\n", datei);
		if (y)
			meldungAusgeben("Datei konnte nicht geöffnet werden.\n");
		return 0;
	} else {
		int i, j;
		/*
		 * Das Sudoku wird Zeilenweise bearbeitet
		 */
		for (i = 0; i < HOEHE; i++) {
			/*
			 * Sudokuzeile wird Zeichen für Zeichen in die Datei übertragen
			 */
			for (j = 0; j < BREITE; j++) {
				fputc(asFeld(feld[i][j]), fp);
			}
			/*
			 * Am Ende der Zeile wird ein Leerzeichen geschrieben
			 */
			fputc(10, fp);

		}
		fclose(fp);
		return 1;
	}
}
