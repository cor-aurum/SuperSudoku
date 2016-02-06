#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int HOEHE = 9;
int BREITE = 9;
int KACHELHOEHE = 3;
int KACHELBREITE = 3;
int MAX_ZAHL = 9;

/* Sudoku.c */
int main(int argc, char* argv[]); // Main Methode
int setFeld(int x, int y, int eingabe, int lock); // Position x, y im Feld setzen
int starteGUI();
int** allokieren(int y, int x);

/* Writer.c */
int dateiExistiert(char datei[]); // Prüft ob die Datei existiert (1) oder nicht (0)
int speichereFeld(char datei[], int y); // Speichert das aktuelle Feld aus dem RAM in eine Datei

/* Reader.c */
int leseDatei(char *dateipfad);

/* Tester.c */
int pruefePos(int **feld, int x, int y, int wert);
int testSudokuFormal(int **feld);
int pruefeVollstaendig(int **feld);

/* Writer.c */

/* Solver.c */
int loese(int **feld, int x, int y);
int loeseAbwaerts(int **feld, int x, int y);
int eindeutig(int **feld);
int loeseSudoku(int **feld);

/* Generator.c */
void generiereSudoku(int **feld, int schwierigkeit);

/* GUI */

