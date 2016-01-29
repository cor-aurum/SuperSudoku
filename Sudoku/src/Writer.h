/*
 * Diese Methode schreibt ein im RAM gespeichertes Sudoku in die Datei datei[].
 */
void speichereFeld(char datei[]) {
	FILE *fp;

	fp = fopen(datei, "w");

	if (fp == NULL) {
		meldungAusgeben("Datei konnte nicht geöffnet werden.\n");
	} else {
		int i, j;
		for (i = 0; i < HOEHE; i++) {
			for (j = 0; j < BREITE; j++) {
				if (feld[i][j] == 0) {
					fputc(' ', fp);
				} else {
					fputc(feld[i][j] + 48, fp);
				}
			}
			fputc(10, fp);

		}
		fclose(fp);
	}
}
