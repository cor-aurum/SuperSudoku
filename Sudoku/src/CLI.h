/**
 * k	Korrekt
 * l	Lösen
 * p	Speichern
 * o	Öffnen
 * i	Einlesen
 * e	Eindeutigkeit prüfen
 * g	Generieren
 * u	Über das Programm
 * h	Hilfe
 */

void o_help() {
	printf("\n");
	printf("   Anwendung:  sudoku [-o[a|l|x|y]] [Quelldatei]                        \n");
	printf("               sudoku [-e|-f|-l|-k|-h|-g<N>] [Quelldatei] [Zieldatei]   \n");
	printf("                                                                        \n");
	printf("   Optionen:                                                            \n");
	printf("      -e       Gibt an, ob das Sudoku eindeutig lösbar ist.             \n");
	printf("      -f       Datei auf Formatkonvention prüfen.                       \n");
	printf("      -g<N>    Generiert ein Sudoku des Schwierigkeitsgrades            \n");
	printf("                  N mit N zwischen 1 (leicht) und 9 (schwer)            \n");
	printf("                  Hinweis: Das kann eine Weile dauern.                  \n");
	printf("      -h       Hilfe ausgeben.                                          \n");
	printf("      -u       About-Page ausgeben.                                     \n");
	printf("      -k       Gibt an, ob das Sudoku formale Fehler hat.               \n");
	printf("      -bN[M]   Initialisiert das Sudoku mit der Kachelbreite N,         \n");
	printf("                     und N*N oder, wenn angegeben M Kacheln             \n");
	printf("                     Achtung: Große Sudokus können SEHR langsam sein    \n");
	printf("      -o       Öffnet ein Sudoku in der Oberfläche.                     \n");
	printf("               Erlaubt die Zusätzlich Parameter l,y,x,a,b\n             \n");
	printf("      -l|l     Löst das Sudoku, wenn es lösbar ist.                     \n");
	printf("       y       Einlesen und Felder schützen.                            \n");
	printf("       x       Einlesen ohne geschützte Felder.                         \n");
	printf("       a       Zeichensatz ohne Unicode erzwingen                       \n");
	printf("       b       Fetter Zeichensatz                                     \n\n");
	printf("       Schalter werden von hinten nach vorne bearbeitet!              \n\n");
	printf("    Super Sudoku by Moritz Koch, Felix Schütze und Sascha Scherrer.   \n\n");
}

void argumentInterpreter(int argc, char* argv[]) {
	char *datei = "";
	char *datei2 = "";
	int i, j, k,l;
	if (argc == 1) {
		starteGUI();
	}
	for (i = argc - 1; i >= 0; i--) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'h':
				o_help();
				break;
			case 'e':
				if (leseDatei(datei) < 0) {
					printf("Benutzung: sudoku -e Quelldatei\n");
					break;
				}
				printf("Sudoku %s ist %seindeutig\n", datei,
						eindeutig(feld) ? "" : "nicht ");
				break;
			case 'j':
				printf("%s\n", getJoke());
				break;
			case 'u':
				printUber(0);
				break;
			case 'o':
				leseDatei(datei);
				for (j = 1; j < (int) strlen(argv[i]); j++) {
					if (argv[i][j] == ' ')
						break;
					if (argv[i][j] == 'x') {
						for (k = 0; k < BREITE; k++) {
							for (l = 0; l < HOEHE; l++) {
								/* schutz > 0 ist lock, schutz < 0 ist fehler */
								if (schutz[k][l] > 0)
									schutz[k][l] = 0;
							}
						}
					}
					if (argv[i][j] == 'y') {
						for (k = 0; k < BREITE; k++) {
							for (l = 0; l < HOEHE; l++) {
								/* schutz > 0 ist lock, schutz < 0 ist fehler */
								if (schutz[k][l] == 0)
									schutz[k][l] = 1;
							}
						}
					}
					if (argv[i][j] == 'l') {
						leseDatei(datei);
						if (loeseSudoku(feld))
							printf("Sudoku %s gelöst.\n", datei);
						else
							printf("Sudoku %s nicht lösbar.\n", datei);
					}
					if (argv[i][j] == 'a') {
						zeichensatz = 1;
					}
					if (argv[i][j] == 'b') {
						zeichensatz = 2;
					}
				}
				starteGUI();
				break;
			case 'l':
				if (leseDatei(datei) < 0) {
					printf("Benutzung: sudoku -l Quelldatei Zieldatei\n");
					break;
				}
				if (loeseSudoku(feld))
					printf("Sudoku %s gelöst.\n", datei);
				else
					printf("Sudoku %s nicht lösbar.\n", datei);
				if (!speichereFeld(datei2, 0)) {
					printf("Benutzung: sudoku -l Quelldatei Zieldatei\n");
				}
				break;
			case 'g': {
				int schwierigkeit = 1;
				if (argv[i][2] >= '1' && argv[i][2] <= '9') {
					schwierigkeit = argv[i][2] - '0';
				}
				generiereSudoku(feld, schwierigkeit);
				if (!speichereFeld(datei, 0))
					printf("Benutzung: sudoku -g<N> Zieldatei\n");
				break;
			}
			case 'f': {
				int leseReturn = leseDatei(datei);
				if (leseReturn < 0) {
					fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", datei);
				} else {
					printf("Die Datei %s beinhaltet %d Fehler.\n", datei, leseReturn);
				}
				break;
			}
			case 'k': {
				if (leseDatei(datei) < 0) {
					fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", datei);
				} else {
					if (!testSudokuFormal(feld)) {
						printf("Sudoku %s ist formal korrekt.\n", datei);
					} else {
						printf("Sudoku %s hat formale Fehler.\n", datei);
					}
				}
				break;
			}
			case 'b': {
				int wert = 3;
				int seite = wert * wert;
				if (argv[i][2] >= '1' && argv[i][2] <= '9') {
					wert = argv[i][2] - '0';
					seite = wert * wert;
					if (argv[i][3] >= wert + '0' && argv[i][3] <= '9') {
						seite = wert * (argv[i][3] - '0');
					}
				}

				KACHELBREITE = wert;
				KACHELHOEHE = wert;
				HOEHE = seite;
				BREITE = seite;
				MAX_ZAHL = seite;
				feld = allokieren(BREITE, HOEHE);
				schutz = allokieren(BREITE, HOEHE);
				zahlen = malloc(MAX_ZAHL * sizeof(int *));
				if (NULL == zahlen) {
					printf("Kein virtueller RAM mehr vorhanden ... !");
				}
				break;
			}
			default:
				printf("Ungültige Operation -%c, -h für weitere Informationen.\n",
						argv[i][1]);
			}
		} else {
			datei2 = datei;
			datei = argv[i];
		}
	}
}
