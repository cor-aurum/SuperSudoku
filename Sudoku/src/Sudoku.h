#include <stdio.h>
#include <stdlib.h>

#define HOEHE 9
#define BREITE 9
#define KACHELHOEHE 3
#define KACHELBREITE 3
#define MAX_ZAHL 9

/* Sudoku.c */
int main(int argc, char* argv[]); // Main Methode
int setFeld(int x, int y, int eingabe, int lock); // Position x, y im Feld setzen
int starteGUI();

/* Writer.c */
int dateiExistiert(char datei[]); // Prüft ob die Datei existiert (1) oder nicht (0)
int speichereFeld(char datei[]); // Speichert das aktuelle Feld aus dem RAM in eine Datei

/* Reader.c */
int leseDatei(char *dateipfad);

/* Tester.c */
int pruefePos(int feld[BREITE][HOEHE], int x, int y, int wert);
int testSudokuFormal(int feld[BREITE][HOEHE]);

/* Writer.c */

/* Solver.c */
int loese(int feld[BREITE][HOEHE], int x, int y);
int loeseAbwaerts(int feld[BREITE][HOEHE], int x, int y);
int eindeutig(int feld[BREITE][HOEHE]);
int loeseSudoku(int feld[BREITE][HOEHE]);

/* Generator.c */
void generiereSudoku(int feld[BREITE][HOEHE], int schwierigkeit);

/* GUI */

