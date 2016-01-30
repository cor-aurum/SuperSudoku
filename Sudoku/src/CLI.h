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

/*
 * Prüft, ob das übergebene Argument eine Option ist
 * returns 1, wenn die Option gegeben ist.
 * returns 0, wenn die Option nicht gesetzt ist.
 * Quelle: http://openbook.rheinwerk-verlag.de
 * 			/c_von_a_bis_z/013_c_kommandozeilenargumente_002.htm
 */
int optionGesetzt(char *argument, char *option) {
	if (argument[0] == '-' && argument[1] == option[0])
		return 1;
	else
		return 0;
}

void o_help() {
	printf("\n");
	printf("   Anwendung:  sudoku [-h|-g<N>]                             \n");
	printf("               sudoku [-e|-f|-i|-l|-k|-x] Quelldatei         \n");
	printf("               sudoku -g<N> [Zieldatei]                      \n");
	printf("               sudoku Quelldatei [Zieldatei]                 \n");
	printf("                                                             \n");
	printf("   Optionen:                                                 \n");
	printf("      -e       Gibt an, obd as Sudoku eindeutig lösbar ist.  \n");
	printf("      -f       Datei auf Formatkonvention prüfen.            \n");
	printf("      -g<N>    Generiert ein Sudoku des Schwierigkeitsgrades \n");
	printf("                  N mit N zwischen 1 (leicht) und 9 (schwehr)\n");
	printf("                  Hinweis: Das kann eine Weile dauern.       \n");
	printf("      -h       Hilfe ausgeben.                               \n");
	printf("      -k       Gibt an, ob das Sudoku formale Fehler hat.    \n");
	printf("      -l       Löst das Sudoku, wenn es lösbar ist.          \n");
	printf("      -x       Einlesen ohne geschützte Felder.              \n\n");
	printf("   Sudoku by Moritz Koch, Felix Schütze und Sascha Scherrer. \n\n");
}

/*
 *
 */
void argumentInterpreter(int argc, char* argv[]) {
	/*
	 * TODO: Generiere Sudoku ist nicht zufällig, sonder immer das gleiche Feld
	 * 		 Das Problem liegt also wahrscheinlich am Zufall.
	 *
	 *
	 */
	if (argc == 1) {
		// Kein Argument: GUI starten:
		starteGUI();
	} else if (argc == 2) {
		// Hilfe ausgeben:
		if (optionGesetzt(argv[1], "h")) {
			printf("Hilfe für SUDOKU");
			o_help();
		} else
		// Sudoku generieren und in Interaktiven Modus gehen:
		if (optionGesetzt(argv[1], "g")) {

			if (argv[1][2] != '\0') {
				if (argv[1][2] >= '1' && argv[1][2] <= '9') {
					fprintf(stdout,
							"Sudoku wird generiert. Das kann einen Moment dauern...\n");
					generiereSudoku(feld, argv[1][2] - '0');
					starteGUI();
				} else {
					fprintf(stderr,
							"Der Schwierigkeitsgrad muss zwischen 1 und 9 liegen.\n");
				}
			} else {
				generiereSudoku(feld, 1);
				starteGUI();
			}
		} else
		// Nichts besonderes:
		if (optionGesetzt(argv[1], "j")) {
			printf("%s\n", getJoke());
		} else {
			// Sudoku aus Datei einlesen.
			if (leseDatei(argv[1]) < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n",
						argv[1]);
			} else {
				starteGUI();
			}
		}
	} else if (argc == 3) {
		// Sudoku generieren und in Datei speichern.
		if (optionGesetzt(argv[1], "g")) {
			if (argv[1][2] != '\0') {
				if (argv[1][2] >= '1' && argv[1][2] <= '9') {
					fprintf(stdout,
							"Sudoku wird generiert. Das kann einen Moment dauern...\n");
					generiereSudoku(feld, argv[1][2] - '0');
					speichereFeld(argv[2]);
				} else {
					fprintf(stderr,
							"Der Schwierigkeitsgrad muss zwischen 1 und 9 liegen.\n");
				}
			} else {
				generiereSudoku(feld, 1);
				speichereFeld(argv[2]);
			}
		} else
		// Sudoku aus Datei einlesen
		if (optionGesetzt(argv[1], "i")) {
			if (leseDatei(argv[2]) < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n",
						argv[2]);
			} else {
				starteGUI();
			}
		} else
		// Sudoku einlesen und Lösung in GUI anzeigen
		if (optionGesetzt(argv[1], "l")) {
			if (leseDatei(argv[2]) < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n",
						argv[2]);
			} else {
				loeseSudoku(feld);
				starteGUI();
			}
		} else
		// Sudoku aus Datei lesen und Schreibschutz aufheben
		if (optionGesetzt(argv[1], "x")) {
			if (leseDatei(argv[2]) < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n",
						argv[2]);
			} else {
				for (int i = 0; i < BREITE; i++) {
					for (int j = 0; j < HOEHE; j++) {
						/* schutz > 0 ist lock, schutz < 0 ist fehler */
						if (schutz[i][j] > 0)
							schutz[i][j] = 0;
					}
				}
				starteGUI();
			}
		} else
		// Sudoku einlesen und auf eindeutigkeit Prüfen
		if (optionGesetzt(argv[1], "e")) {
			if (leseDatei(argv[2]) < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n",
						argv[2]);
			} else {
				if (!loeseSudoku(feld)) {
					printf("Sudoku ist nicht lösbar.\n");
				} else if (eindeutig(feld)) {
					if (!testSudokuFormal(feld)) {
						if (eindeutig(feld)) {
							printf("Sudoku ist eindeutig lösbar.\n");
						} else {
							printf("Sudoku ist nicht eindeutig lösbar.\n");
						}
					} else {
						printf("Sudoku hat formale Fehler.\n");
					}
				}
			}
		} else
		// Sudoku auf Formale fehler Prüfen
		if (optionGesetzt(argv[1], "k")) {
			if (leseDatei(argv[2]) < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n",
						argv[2]);
			} else {
				if (!testSudokuFormal(feld)) {
					printf("Sudoku ist formal korrekt.\n");
				}else{
					printf("Sudoku hat formale Fehler.\n");
				}
			}
		} else
			// Sudoku-Datei auf Dateiformatkonvention prüfen:
			if (optionGesetzt(argv[1], "f")) {
				int leseReturn = leseDatei(argv[2]);
				if (leseReturn < 0) {
					fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n",
							argv[2]);
				} else {
					printf("Die Datei beinhaltet %d Fehler.\n", leseReturn);
				}
			}
	}

	for (int i = 0; i < argc; i++) {
		printf("Argument %d lautet: \"%s\"\n", i, argv[i]);
	}
}

