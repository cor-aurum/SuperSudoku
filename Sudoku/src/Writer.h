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
		for (i = 0; i < HOEHE; i++) {
			for (j = 0; j < BREITE; j++) {
				fputc(asFeld(feld[i][j]), fp);
			}
			fputc(10, fp);

		}
		fclose(fp);
		return 1;
	}
}
