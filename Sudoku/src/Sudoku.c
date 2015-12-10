/*
 ============================================================================
 Name        : Sudoku.c
 Author      : Sascha Scherrer, Moritz Koch, Felix Schuetze
 Version     :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define HOEHE 9
#define BREITE 9
#define KACHELHOEHE (int)(sqrt(HOEHE))
#define KACHELBREITE (int)(sqrt(BREITE))

int feld[HOEHE][BREITE];
char datei[]="speicherstand.txt";

char asFeld(char c) {
	if (c == 0) {
		return ' ';
	}
	return c + 48;
}

void printFeld() {
	int i, j;
	for (i = 0; i < HOEHE; i++) {
		for (j = 0; j < BREITE; j++) {
			printf("%s%s", (j % 3 == 0 && i != 0) ? "|" : " ",
					(i % KACHELHOEHE == 0) ? "____" : "    ");
		}
		printf("%s\n", (i != 0) ? "|" : "");
		for (j = 0; j < BREITE; j++) {
			printf("%s %c  ", (j % KACHELBREITE == 0) ? "|" : " ",
					asFeld(feld[i][j]));
		}
		printf("|\n");

	}
	for (j = 0; j < BREITE; j++) {
		printf("%s%s", (j % 3 == 0 && i != 0) ? "|" : " ",
				(i % KACHELHOEHE == 0) ? "____" : "    ");
	}
	printf("|\n");
}

void speichereFeld() {
	FILE *fp;

	fp = fopen(datei, "w");

	if (fp == NULL) {
		printf("Datei konnte nicht geoeffnet werden.\n");
	} else {
		int  i,j;
		for (i = 0; i < HOEHE; i++) {
			for (j = 0; j < BREITE; j++) {
				if (feld[i][j] == 0) {
					fputc(' ',fp);
				} else {
					fputc(feld[i][j] + 48, fp);
				}
			}
			fputc(10, fp);

		}
		fclose(fp);
	}
}

int main(void) {
	feld[3][4] = 1;
	feld[7][6] = 5;
	printFeld();
	speichereFeld();
	return EXIT_SUCCESS;
}

