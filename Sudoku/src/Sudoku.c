/*
 ============================================================================
 Name        : Sudoku.c
 Author      : Sascha Scherrer, Moritz Koch, Felix Schuetze
 Version     : 2016-01-29
 ============================================================================
 */
#include "Sudoku.h"

/* GLOBALE ARRAYS ************************************************************/
int **feld, **schutz;

/*****************************************************************************/
#include "Tester.h"
#include "GUI.h"
#include "Reader.h"
#include "Writer.h"
#include "Solver.h"
#include "Generator.h"
#include "CLI.h"

/*
 * Fügt den integer eingabe an die Position x,y im Spielfeld hinzu.
 * Der Wert lock gibt an (Wenn ==0), dass das Feld geändert werden darf.
 * Gibt folgende Werte zurück:
 * -1: Eingabe nicht erfolgreich, da Schreiblock vorliegt.
 * -2: Eingabe nicht erfolgreich, da eingabe zu groß
 *  1: Eingabe erfolgreich
 */
int setFeld(int x, int y, int eingabe, int lock) {
	if (schutz[x][y] <= 0) {
		if (!(eingabe >= 0 && eingabe <= MAX_ZAHL)) {
			return -2;
		}
		schutz[x][y] = lock;
		feld[x][y] = eingabe;
		return 1;
	}
	return -1;
}

int starteGUI() {
#ifdef __linux__
	eingabeLoop();
#else
	if (!SetConsoleOutputCP(65001)) {
		fprintf(stderr, "Konsole konnte nicht initialisiert werden.\n");
		return 0;
	}
	eingabeLoop();
#endif
	//printFeld();
	return 1;
}

int** allokieren(int zeile, int spalte) {
	int i, j, **matrix;
	matrix = malloc(zeile * sizeof(int *));
	if (NULL == matrix) {
		fprintf(stderr, "Nicht genügend Speicher vorhanden\n");
	}
	for (i = 0; i < zeile; i++) {
		matrix[i] = malloc(spalte * sizeof(int));
		if (NULL == matrix[i]) {
			fprintf(stderr, "Nicht genügend Speicher vorhanden\n");
		}
	}
	for (i = 0; i < zeile; i++)
		for (j = 0; j < spalte; j++)
			matrix[i][j] = 0;
	return matrix;
}

/*
 * main Methode, allokiert Speicher für das Feld, für den Schutzspeicher und für das Zahlenarray,
 * welches beim Generieren benötigt wird. Anschließend wird das Command Line Interface gestartet.
 *
 * WICHTIG: zum reibungslosen Ablauf empfehlen wir ein GNU/Linux Betriebssystem. Getestet unter Kubuntu
 */
int main(int argc, char* argv[]) {
	feld = allokieren(BREITE, HOEHE);
	schutz = allokieren(BREITE, HOEHE);
	zahlen = malloc(MAX_ZAHL * sizeof(int *));
	if (NULL == zahlen) {
		fprintf(stderr, "Nicht genügend Speicher vorhanden\n");
	}
	argumentInterpreter(argc, argv);
	return EXIT_SUCCESS;
}
