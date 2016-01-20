/*****************************************************************************
 * SUDOKU ./Generator.h                                                      *
 * Implementierung eines Algorithmus zum Lösen eines vorgegebenen Sudokus    *
 *                                                                           *
 * Version 1 (2016-12-19) Felix Schütze, Sascha Scherrer, Moritz Koch        *
 *****************************************************************************/

int zahlen[MAX_ZAHL] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

void initZufall(int *array) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int usec = tv.tv_usec;
	srand48(usec);
	size_t i;
	for (i = 8; i > 0; i--) {
		size_t j = (unsigned int) (drand48() * (i + 1));
		int t = array[j];
		array[j] = array[i];
		array[i] = t;
	}
}

void fuelleZufall(int feld[BREITE][HOEHE]) {
	int i, j, k, l = 0;
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			feld[i][j] = 0;
			schutz[i][j] = 0;
		}
	}
	for (i = 0; i < MAX_ZAHL; i += KACHELBREITE) {
		initZufall(zahlen);
		l = 0;
		for (j = 0; j < KACHELBREITE; j++) {
			for (k = 0; k < KACHELHOEHE; k++) {
				feld[j + i][k + i] = zahlen[l++];
			}
		}
	}
	loeseSudoku(feld);
}

int pruefeMehrdeutigkeit() {
	return -1;
}

void generiereSudoku(int feld[BREITE][HOEHE]) {
	int i;
	fuelleZufall(feld);
	for (i = 0; i < MAX_ZAHL; i++) {
		feld[i][rand() % MAX_ZAHL] = 0;
	}
}
