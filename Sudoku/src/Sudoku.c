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

/* HEADERDATEIEN *************************************************************/
#include "GUI.h"
#include "Reader.h"
#include "Solver.h"

/*
 * Globale Variable für die Speicherung des Sudokus
 */
int feld[BREITE][HOEHE]= {
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0}
	};

int kopiereArray(int *quelle, int *ziel) {
	int x, y;
	for (x = 0; x < BREITE; x++) {
		for (y = 0; y < HOEHE; y++) {
			//ziel[x][y] = quelle[x][y];
		}
	}
	return 1; // Erfolg
}

/*
 * Prüft, ob Sudoku formal korrekt ist.
 * Rückgabe:
 * 1: Sudoku ist korrekt
 * 0: Sudoku formal inkorrekt
 * TODO funktioniert noch nicht!
 */
int checkSudokuFormal() {
	return 0;
}

/*
 * Prüft, ob Sudoku lösbar ist.
 * Rückgabe:
 * 1: Sudoku ist korrekt
 * 0: Sudoku nicht lösbar
 * TODO funktioniert noch nicht!
 */
int checkSudokuLoesbar() {
	return 0;
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
 * Diese Methode gibt einen Wert im Sudoku als Druckbares Zeichen zurück.
 */
char asFeld(char c) {
	if (c == 0) {
		return ' ';
	}
	return c + 48;
}

/*
 * Diese Methode schreibt in den standartout eine gut lesbare Darstellung des Sudokus
 */
void printFeld() {
	int i, j;
	for (i = 0; i < HOEHE; i++) {
		for (j = 0; j < BREITE; j++) {
			printf("%s%s", (j % 3 == 0 && i != 0) ? "|" :(i % 3 != 0) ? ".": " ",
					(i % KACHELHOEHE == 0) ? "____" : "    ");
		}
		printf("%s\n", (i != 0) ? "|" : "");
		for (j = 0; j < BREITE; j++) {
			printf("%s %c  ", (j % KACHELBREITE == 0) ? "|" : " ",
					asFeld(feld[i][j]));

		}
		printf("|     ");
		switch (i) {
		case 0:
			printf("wasd: Cursor bewegen");
			break;
		case 1:
			printf("Leerzeichen: Zahl löschen");
			break;
		case 2:
			printf("1-9: Zahl eintragen");
			break;
		case 3:
			printf("p: Spiel speichern");
			break;
		case 4:
			printf("o: Spiel laden");
			break;
		case 8:
			printf("q: Programm beenden");
			break;
		}
		printf("\n");

	}
	for (j = 0; j < BREITE; j++) {
		printf("%s%s", (j % 3 == 0 && i != 0) ? "|" : " ",
				(i % KACHELHOEHE == 0) ? "____" : "    ");
	}
	printf("|\n");
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
	printf("Sudoku Reader\n");
	//	printf("Antwort: %d \n", rh_frageJaNein("Frage", 1));
	//	printf("Antwort: %d \n", rh_frageJaNein("Frage", 0));
	//	printf("Antwort: %d \n", rh_frageJaNein("Frage", 1));
	//leseDatei("sudoku.txt");
	//leseDatei("sudoku_err.txt");
	//leseDatei("sudoku_err2.txt");
	//leseDatei("sudoku_err3.txt");
	leseDatei(datei);
	printf("Done.\n");
	return 0;
}

/*
 * TODO Moritz implementiert hier den Algorithmus für das Lösen des Sudokus. Gibt den Erfolg zurück.
 */
int loeseSudokuMain() {
	printf("SUDOKU ./solver.c \n");
	ausgabeFeld(feld);
	loeseSudoku(feld);
	ausgabeFeld(feld);
	return 0;
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
	static int schutz[BREITE][HOEHE];
	if (!schutz[x][y]) {
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
int main(void) {
	//loeseSudokuMain();
	eingabe(2);
	//printFeld();
	return EXIT_SUCCESS;
}
