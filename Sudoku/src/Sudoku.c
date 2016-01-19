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

/* PRÄPROZESSORDIREKTIVEN *****************************************************/
#define HOEHE 9
#define BREITE 9
#define KACHELHOEHE 3
#define KACHELBREITE 3
#define MAX_ZAHL 9

int feld[BREITE][HOEHE];
int schutz[BREITE][HOEHE];

/* HEADERDATEIEN *************************************************************/
#include "GUI.h"
#include "Reader.h"
#include "Solver.h"
#include "Generator.h"

/*
 * Prüft, ob Sudoku formal korrekt ist.
 * Rückgabe:
 * 1: Sudoku ist korrekt
 * 0: Sudoku formal inkorrekt
 */
int checkSudokuFormal() {
	return testSudokuFormal(feld);
}

/*
 * Prüft, ob Sudoku, lösbar ist und ob Sudoku formal korrekt ist.
 * Rückgabe:
 * 1: Sudoku ist korrekt
 * -1: Sudoku formal inkorrekt
 * -2: Sudoku nicht lösbar
 * TODO funktioniert noch nicht!
 */
int checkSudoku() {
	return 0;
}

/*
 * Diese Methode schreibt ein im RAM gespeichertes Sudoku in die Datei datei[].
 */
void speichereFeld(char datei[]) {
	FILE *fp;

	fp = fopen(datei, "w");

	if (fp == NULL) {
		printf("Datei konnte nicht geoeffnet werden.\n");
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
 * TODO Sascha implementiert hier eine Methode, welche aus einer Datei ein Spielfeld ausliest. Die einzelnen Werte werden mittels setFeld(param) eingespeichert.
 *  Gibt im erfolgreichen Fall 1 zurück, andernfalls 0.
 */
int leseFeldAusDatei(char datei[]) {
	//printf("Sudoku Reader\n");
	//	printf("Antwort: %d \n", rh_frageJaNein("Frage", 1));
	//	printf("Antwort: %d \n", rh_frageJaNein("Frage", 0));
	//	printf("Antwort: %d \n", rh_frageJaNein("Frage", 1));
	//leseDatei("sudoku.txt");
	//leseDatei("sudoku_err.txt");
	//leseDatei("sudoku_err2.txt");
	//leseDatei("sudoku_err3.txt");
	return !leseDatei(datei);
	//printf("Done.\n");
}

/*
 * TODO Moritz implementiert hier den Algorithmus für das Lösen des Sudokus. Gibt den Erfolg zurück.
 */
int loeseSudokuMain() {
	loeseSudoku(feld);
	return 1;
}

/*
 * TODO Felix implementiert hier die Methode zur Usereingabe in der Konsole
 * Modi:
 * 1: Leeres Sudoku wird bearbeitet, Achtung, feld[] wird überschrieben
 * 2: Ein vorgegebenes Sudoku wird bearbeitet, beachtet Schreiblock nicht
 * 3: Ein vorgegebenes Sudoku wird ausgefüllt, beachtet Schreiblock, Standard für ein einfaches Spiel
 */
void eingabe(int modus) {
	eingabeLoop();
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
 * Gibt den Wert an der Stelle x,y des Sudokus zurück, liegt der x,y außerhalb des Sudokus ist das Ergebnis negativ
 */
int getFeld(int x, int y) {
	if (x < HOEHE && x > 0 && y < BREITE && y > 0) {
		return feld[x][y];
	}
	return -1;
}

/*
 * main Methode, die darf jemand anderes kommentieren
 */
int main(int argc, char* argv[]) {
#if !defined(WIN32)
	eingabe(2);
#else
	if(!SetConsoleOutputCP(65001))
	{
		printf("Fehler\n");
		return 0;
	}
	eingabe(2);
#endif
	//printFeld();
	return EXIT_SUCCESS;
}
