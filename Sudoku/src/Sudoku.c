/*
 ============================================================================
 Name        : Sudoku.c
 Author      : Sascha Scherrer, Moritz Koch, Felix Schuetze
 Version     : 2016-01-29
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/* PRÄPROZESSORDIREKTIVEN *****************************************************/
#define HOEHE 9
#define BREITE 9
#define KACHELHOEHE 3
#define KACHELBREITE 3
#define MAX_ZAHL 9

/* GLOBALE ARRAYS ************************************************************/
int feld[BREITE][HOEHE];
int schutz[BREITE][HOEHE];

/* HEADERDATEIEN *************************************************************/
#include "Sudoku.h"

#include "GUI.h"
#include "Reader.h"
#include "Solver.h"
#include "Generator.h"

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



/*
 * Fügt den integer eingabe an die Position x,y im Spielfeld hinzu.
 * Der Wert lock gibt an (Wenn ==0), dass das Feld geändert werden darf.
 * Gibt folgende Werte zurück:
 * -1: Eingabe nicht erfolgreich, da Schreiblock vorliegt.
 * -2: Eingabe nicht erfolgreich, da eingabe zu groß
 * -3: Eingabe widerspricht Sudokuregeln (TODO)
 *  1: Eingabe erfolgreich
 */
int setFeld(int x, int y, int eingabe, int lock) {
	if (schutz[x][y]<=0) {
		if (!(eingabe >= 0 && eingabe <= MAX_ZAHL)) {
			return -2;
		}
		schutz[x][y] = lock;
		feld[x][y] = eingabe;
		return 1;
	}
	return -1;
}



/*
 * main Methode, die darf jemand anderes kommentieren
 */
int main(int argc, char* argv[]) {
#if !defined(WIN32)
	eingabeLoop();
#else
	if(!SetConsoleOutputCP(65001))
	{
		printf("Fehler\n");
		return 0;
	}
	eingabeLoop();
#endif
	//printFeld();
	return EXIT_SUCCESS;
}
