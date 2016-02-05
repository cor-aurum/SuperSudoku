/*****************************************************************************
 * SUDOKU ./Generator.h                                                      *
 * Implementierung eines Algorithmus zum Lösen eines vorgegebenen Sudokus    *
 *                                                                           *
 * Version 1 (2016-12-19) Felix Schütze, Sascha Scherrer, Moritz Koch        *
 *****************************************************************************/

int *zahlen;

void initZufall(int *array) {
	if (MAX_ZAHL > 1) {
		size_t i;
		for (i = 0; i < MAX_ZAHL - 1; i++) {
			size_t j = i + rand() / (RAND_MAX / (MAX_ZAHL - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

void fuelleZufall(int **feld) {
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

void generiereSudoku(int **feld, int schwierigkeit) {
	int x, y, z, mem;
	for(x=0;x<MAX_ZAHL;x++)
	{
		zahlen[x]=x+1;
	}
	// Feld ausfüllen
	fuelleZufall(feld);

	// Zufällig ein Wert je Zeile leeren
	for (x = 0; x < BREITE; x++) {
		feld[x][rand() % HOEHE] = 0;
	}

	for (z = 0; z < MAX_ZAHL * schwierigkeit; z++) {
		do {
			x = rand() % MAX_ZAHL;
			y = rand() % MAX_ZAHL;
			mem = feld[x][y];
			feld[x][y] = 0;
		} while (eindeutig(feld));
		feld[x][y] = mem;
	}
	for (x = 0; x < BREITE; x++) {
		for (y = 0; y < HOEHE; y++) {
			if (feld[x][y]) {
				schutz[x][y] = 1;
			}
		}
	}
}
