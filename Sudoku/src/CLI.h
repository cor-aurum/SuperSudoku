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
	printf("   Anwendung:  sudoku [-h|-g<N>]                                      \n");
	printf("               sudoku [-e|-f|-i|-l|-k|-x] Quelldatei                  \n");
	printf("               sudoku -g<N> [Zieldatei]                               \n");
	printf("               sudoku Quelldatei [Zieldatei]                          \n");
	printf("                                                                      \n");
	printf("   Optionen:                                                          \n");
	printf("      -e       Gibt an, ob das Sudoku eindeutig lösbar ist.           \n");
	printf("      -f       Datei auf Formatkonvention prüfen.                     \n");
	printf("      -g<N>    Generiert ein Sudoku des Schwierigkeitsgrades          \n");
	printf("                  N mit N zwischen 1 (leicht) und 9 (schwer)          \n");
	printf("                  Hinweis: Das kann eine Weile dauern.                \n");
	printf("      -h       Hilfe ausgeben.                                        \n");
	printf("      -u       About Page ausgeben.                                   \n");
	printf("      -k       Gibt an, ob das Sudoku formale Fehler hat.             \n");
	printf("      -l       Löst das Sudoku, wenn es lösbar ist.                   \n");
	printf("      -x       Einlesen ohne geschützte Felder.                     \n\n");
	printf("    Super Sudoku by Moritz Koch, Felix Schütze und Sascha Scherrer. \n\n");
}

void argumentInterpreter(int argc, char* argv[]) {
	char *datei = "";
	char *datei2 = "";
	if (argc == 1) {
		starteGUI();
	}
	for (int i = argc - 1; i >= 0; i--) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'h':
				o_help();
				break;
			case 'e':
				leseDatei(datei);
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
				if (argv[i][2] == 'x') {
					for (int i = 0; i < BREITE; i++) {
						for (int j = 0; j < HOEHE; j++) {
							/* schutz > 0 ist lock, schutz < 0 ist fehler */
							if (schutz[i][j] > 0)
								schutz[i][j] = 0;
						}
					}
				}
				starteGUI();
				break;
			case 'l':
				leseDatei(datei);
				if (loeseSudoku(feld))
					printf("Sudoku %s gelöst.\n", datei);
				else
					printf("Sudoku %s nicht lösbar.\n", datei);
				speichereFeld(datei2);
				break;
			case 'g': {
				int schwierigkeit=1;
				if(argv[i][2] >= '1'&& argv[i][2]<='9')
				{
					schwierigkeit=argv[i][2]-'0';
				}
				generiereSudoku(feld,schwierigkeit);
				speichereFeld(datei);
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

/*
 *
 */
void argumentInterpreter2(int argc, char* argv[]) {
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
					generiereSudoku(feld, (int) (argv[1][2] - '0'));
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
			if (optionGesetzt(argv[1], "u")) {
				printUber(0);
			} else {
				// Sudoku aus Datei einlesen.
				if (leseDatei(argv[1]) < 0) {
					fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", argv[1]);
				} else {
					starteGUI();
				}
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
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", argv[2]);
			} else {
				starteGUI();
			}
		} else
		// Sudoku einlesen und Lösung in GUI anzeigen
		if (optionGesetzt(argv[1], "l")) {
			if (leseDatei(argv[2]) < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", argv[2]);
			} else {
				loeseSudoku(feld);
				starteGUI();
			}
		} else
		// Sudoku aus Datei lesen und Schreibschutz aufheben
		if (optionGesetzt(argv[1], "x")) {
			if (leseDatei(argv[2]) < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", argv[2]);
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
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", argv[2]);
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
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", argv[2]);
			} else {
				if (!testSudokuFormal(feld)) {
					printf("Sudoku ist formal korrekt.\n");
				} else {
					printf("Sudoku hat formale Fehler.\n");
				}
			}
		} else
		// Sudoku-Datei auf Dateiformatkonvention prüfen:
		if (optionGesetzt(argv[1], "f")) {
			int leseReturn = leseDatei(argv[2]);
			if (leseReturn < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", argv[2]);
			} else {
				printf("Die Datei beinhaltet %d Fehler.\n", leseReturn);
			}
		} else
		// Sudoku aus erster Datei lesen und in zweite Datei lösen
		if (argv[1][0] != '-') {
			int leseReturn = leseDatei(argv[1]);
			if (leseReturn < 0) {
				fprintf(stderr, "Laden der Datei %s fehlgeschlagen.\n", argv[1]);
			} else {
				if (leseReturn > 0) {
					fprintf(stderr,
							"Die Datei beinhaltet %d Fehler. Die Fehler wurden automatisch"
									" korrigiert. \n"
									"Das kann dazu führen, dass das Soduku nicht so "
									"interpretiert wird, wie es gedacht war.\n"
									"Bitte überprüfen Sie die "
									"Quelldatei und versuchen Sie es anschließend erneut.\n",
							leseReturn);
				}
				if (!testSudokuFormal(feld)) {
					if (!loeseSudoku(feld)) {
						fprintf(stderr, "Das Sudoku kann nicht gelöst werden.\n   "
								"Grund: Sudoku ist nicht lösbar.\n");
					} else

					if (eindeutig(feld)) {
						printf("Das Sudoku ist eindeutig lösbar. \n");
						if (speichereFeld(argv[2])) {
							printf("Die Lösung wurde unter %s gespeichert.\n", argv[2]);
						} else {
							fprintf(stderr, "Die angegebene Lösungsdatei %s"
									"konnte nicht geschrieben werden.\n", argv[2]);
						}
					} else {
						printf("Das Sudoku ist nicht eindeutig lösbar. \n");
						if (speichereFeld(argv[2])) {
							printf("Eine Mögliche Lösung wurde unter %s gespeichert.\n",
									argv[2]);
						} else {
							fprintf(stderr, "Die angegebene Lösungsdatei %s"
									"konnte nicht geschrieben werden.\n", argv[2]);
						}
					}
				} else {
					fprintf(stderr, "Das Sudoku kann nicht gelöst werden.\n   "
							"Grund: Sudoku in Eingabedatei weist formale Fehler auf.\n");
				}

			}
		}
	} else {
		fprintf(stderr,
				"Für die angegebenen Argumente konnte keine Interpretation gefunden werden.\n");
		printf("   Die Interpretation ihrer Eingabe lautet: \n");
		for (int i = 0; i < argc; i++) {
			printf("      Argument %d: \"%s\"\n", i, argv[i]);
		}
		printf("   Mit dem Schalter \"-h\" erhalten Sie eine Befehlsübersicht.\n");
	}

}

