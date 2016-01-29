/******************************************************************************
 * SUDOKU ./reader.c                                                          *
 * Einlesefunktion für Textdateien mit Sudoku Rätzeln gemäß der Vorgaben      *
 * Ungetestet nicht  produktionsreif, Fehlerkorrektur noch verbessbar         *
 * Version 2 (2016-01-29) Sascha Scherrer <dhbw.scherrer@gmail.com>           *
 ******************************************************************************/

int pruefe(int feld[BREITE][HOEHE], int x, int y, int zaehler);

/* HILFSFUNKTIONEN (Präfix 'rh_' für reader heĺper) ***************************/

/*
 * Hilfsfunktion rh_fehlerZaehler(int typ, anzahlNeueFehler)
 * Zählt die Anzahl an Fehlern, die während des Einlesens gefunden werden.
 * return -1 - wenn der Typ nicht im zugelassenen Wertebereich ist.
 * return n - die aktuelle anzahl an Fehlern des angegebenen Typ
 */
int rh_fehlerZaehler(int typ, int anzahlNeueFehler) {
	static int anzahlFehler[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//printf("Fehlerzaehler Typ=%d, Inkrement=%d \n", typ, anzahlNeueFehler);
	if (typ >= 0 && typ <= 8) {
		anzahlFehler[typ] += anzahlNeueFehler;
		return anzahlFehler[typ];
	}
	return -1;
}

/*
 * Hilfsfunktion rh_fehlerZaehlerReset()
 * Setzt den fehlerZaehler zurück.
 * Siehe rh_fehlerZaehler. 
 */
void rh_fehlerZaehlerReset() {
	int typ;
	for (typ = 0; typ < 8; typ++)
		rh_fehlerZaehler(typ, -rh_fehlerZaehler(typ, 0));
}

/*
 * Hilfsfunktion rh_zaehlerReset(int[] array, int laenge, int wert)
 * Setzt alle elemente des int-arrays auf den angegebenen Wert
 * int[] array - das array (nur eindimensionale arrays)
 * laenge - die anzahl der Elemente in dem Array
 * wert - der Wert, auf den jedes Feld gesetzt wird
 */
void rh_resetArray1D(int array[], int laenge, int wert) {
	int i = 0;
	for (i = 0; i < laenge; i++)
		array[i] = wert;
}

/*
 * Hilfsfunktion rh_resetZaehler(int zaehler[]);
 * Setzt alle Werte von zaehler (muss übergeben werden) auf 0.
 */
void rh_resetZaehler(int zaehler[]) {
	rh_resetArray1D(zaehler, MAX_ZAHL + 1, 0);
}

/*
 * Hilfsfunktion rh_inkrementZaehler(int[] zaehler, int wert)
 * Arbeitet auf dem Array, in dem alle zu zählenden Werte enthalten sind.
 * Ist der Wert im Array enthalten, wird er um eins erhöht.
 * int[] zaehler - array zum Zählen. Jedes Element entspricht einem Wert
 * int wert - der wert, der erhöht wird (Bereich: 0 bis MAX_ZAHL)
 * return 0 - wenn etwas schief gegangen ist
 * return n - die neue Anzahl (n) des betreffenden Wertes (wert)
 */
int rh_inkrementZaehler(int zaehler[], int wert) {
	if (wert >= 0 && wert <= MAX_ZAHL)
		return ++zaehler[wert];
	else {
		/* Unerwartetes Zeichen */
		rh_fehlerZaehler(6, 1);
		return 0;
	}
}

/*
 * Hilfsfunktion  rh_leseDateiZeichenweise(int feld[][], FILE ptr_file)
 * Liest eine Datei Zeichen für zeichen ein, und versucht daraus ein Sudoku
 * zu erstellen.
 * Hinweis: Das einlesen erfolt ungepuffert. Diese Vorhgehensweise ist langsamer als
 *    eine gepufferte Eingabe und sollte daher nicht für große Dateien verwendet werden.
 *    Da wir bei den Sudoku-Dateien von 90 Zeichen (81 Werte und 9 Zeilenumbrüche) ausgegangen,
 *    werden kann, sind die Dateien recht klein, sodass durch das auslassen von Puffern keine
 *    spürbaren verzögerungen zu erwarten sind.
 * Funktionen:
 * - Leerzeichen werden durch "0" (Null) repräsentiert.
 * - Fehlen Zeichen für eine vollständige Reihe, wird die Reihe mit Nullen aufgefüllt.
 * - Alle Zeichen, die keine gültiges Zeichen sind, werden ignoriert.
 *   Gültige Zeichen sind die Ziffern 0 bis 9 und das Leerzeichen. 0 und Leerzeichen sind synonym.
 * - Alle Zeichen, die über die erwartete Zeilenlänge hinausgehen werden ignoriert.
 * - Alle Zeilen, die nach der erwarteten Zeilenzahl folgen, werden ignoriert.
 * - Unerwartete Zeichen im Sudoku werden als unbekannte Ziffer (0 oder Leerzeichen) behandelt.
 * - TODO: Leerzeilen werden ignoriert
 *   Das bedeutet, dass alle Zeilen, die weniger als zwei gültige Zeichen enthalten ignoriert werden.
 * returns 0 - wenn keine Fehler aufgetreten sind
 * returns n - Anzahl der Fehler, die gefunden und korrigiert wurden.
 */
int rh_leseDateiZeichenweise(int feld[BREITE][HOEHE], FILE *ptr_file) {
	char zeichen = 0;
	int posX = 0, posY = 0, anzahlZeichen = 0, erwarteteZeichenzahl = HOEHE
			* BREITE + HOEHE;

	while ((zeichen = fgetc(ptr_file)) != EOF) {
		anzahlZeichen++;

		if (posX < BREITE && posY < HOEHE) {
			/* Zeichen aus Datei in feld übersetzen */
			if (zeichen == ' ')
				feld[posX++][posY] = 0;
			else if (zeichen >= '0' && zeichen <= '9')
				feld[posX++][posY] = zeichen - '0';
		} else if (zeichen == '\n') {
			/* Zeilenumbrüche behandeln */
			if (posX < 1) {
				/* Zeile wird als irrelevant betrachtet, da sie keine gültigen Zeichen enthält */
				rh_fehlerZaehler(1, 1); // korrigierteFehler++
				rh_fehlerZaehler(2, 1); // irrelevanteZeilen++
			} else {
				if (posX != BREITE) {
					rh_fehlerZaehler(1, 1); // korrigierteFehler++
					rh_fehlerZaehler(3, 1); // unpassendeZeichenzahl++
					/* Falsche Anzahl an zeichen korrigieren */
					while (posX < BREITE && posY < HOEHE) {
						feld[posX++][posY] = 0;
						rh_fehlerZaehler(4, 1); // ersetzteZeichen++
					}
				}

				/* Zähler bearbeiten */
				posY++;
				posX = 0;
			}
		} else {
			rh_fehlerZaehler(1, 1); // korrigierteFehler++
			rh_fehlerZaehler(5, 1); // ignorierteZeichen++
		}
	}

	printf("Datei: %d von erwarteten %d Zeichen eingelesen.\n", anzahlZeichen,
			erwarteteZeichenzahl);

	return rh_fehlerZaehler(1, 0);
}

/*
 * Prüft für die durch x und y gegebene Position, ob der wert hier eingesetzt werden darf
 * returns 0 - Wert darf eingesetzt werden
 * returns n - Wert anzahl an Fehlern, die ein einsetzen des Wertes verursachen würde.
 */
int pruefePos(int feld[BREITE][HOEHE], int x, int y, int wert) {
	int fehler = 0, aktX, aktY;

	if (wert == 0)
		return 0; // 0 darf immer eingesetzt werden.

	// In Zeile
	for (aktX = 0; aktX < BREITE; aktX++)
		if (feld[aktX][y] == wert && aktX != x) {
			fehler++;
			fprintf(stderr,
					"   Wert %d in X|Y=%d|%d kommt in Spalte %d mehrmals vor.\n",
					wert, x, y, aktX);
		}
	// In Spalte
	for (aktY = 0; aktY < BREITE; aktY++)
		if (feld[x][aktY] == wert && aktY != y) {
			fprintf(stderr,
					"   Wert %d in X|Y=%d|%d kommt in Zeile %d mehrmals vor.\n",
					wert, x, y, aktY);
			fehler++;
		}

	// TODO: Kachelprüfung ist fehlerhaft.
	// In Kachel
	for (aktX = (x / KACHELBREITE) * KACHELBREITE;
			aktX < (x / KACHELBREITE + 1) * KACHELBREITE; aktX++)
		for (aktY = (y / KACHELHOEHE) * KACHELHOEHE;
				aktY < (y / KACHELHOEHE + 1) * KACHELHOEHE; aktY++)
			if (feld[aktX][aktY] == wert && aktX != x && aktY != y) {
				fehler++;
				fprintf(stderr,
						"   Wert %d in X|Y=%d|%d kommt in Kachel mehrmals vor.\n",
						wert, x, y);
			}

	return fehler;
}

int fehlerMarkieren(int feld[BREITE][HOEHE], int schutz[BREITE][HOEHE], int x,
		int y, int ignSchutz) {
	int aktX = 0, aktY = 0, wert = feld[x][y], markiert = 0;

	// Fehler in gleicher Zeile markieren
	for (aktX = 0; aktX < BREITE; aktX++) {
		if (feld[aktX][y] == wert && (ignSchutz || schutz[aktX][y] <= 0)) {
			schutz[aktX][y] = -wert;
			markiert++;
		}
	}

	// Fehler in gleicher Spalte markeiren
	for (aktY = 0; aktY < HOEHE; aktY++) {
		if (feld[x][aktY] == wert && (ignSchutz || schutz[x][aktY] <= 0)) {
			schutz[x][aktY] = -wert;
			markiert++;
		}
	}

	// Fehler in gleicher Kachel markieren
	for (aktX = (x / KACHELBREITE) * KACHELBREITE;
			aktX < (x / KACHELBREITE + 1) * KACHELBREITE; aktX++)
		for (aktY = (y / KACHELHOEHE) * KACHELHOEHE;
				aktY < (y / KACHELHOEHE + 1) * KACHELHOEHE; aktY++)
			if (feld[aktX][aktY] == wert
					&& (ignSchutz || schutz[aktX][aktY] <= 0)) {
				schutz[aktX][aktY] = -wert;
			}

	return markiert;
}

/* HAUPTFUNKTIONEN ************************************************************/

/*
 * testSudokuFormal(int feld[][])
 * Prüft das Sudoku auf Formale korrektheit und setzt fehlermarkierungen in schutz.
 * returns 0 - wenn das feld formal korrekt ist
 * returns n - die Anzahl der Fehler, die gefunden wurden. 
 */
int testSudokuFormal(int feld[BREITE][HOEHE]) {
	int x = 0, y = 0, f = 0;

	// Fehlerspeicher zurücksetzen (Schutz > 0 ist gelockt, Schutz < 0 ist Fehler)
	for (y = 0; y < HOEHE; y++) {
		for (x = 0; x < BREITE; x++) {
			if (schutz[x][y] < 0)
				schutz[x][y] = 0;
		}
	}

	// Alle gelockten Felder prüfen:
	for (x = 0; x < BREITE; x++) {
		for (y = 0; y < HOEHE; y++) {
			if (schutz[x][y] > 0 && feld[x][y] > 0
					&& pruefePos(feld, x, y, feld[x][y])) {
				// Fehler gefunden:
				// Alle anderen Vorkommen in Reihe, Spalte und Kachel markieren
				f++;

				// Fehler markieren (Schreibschutz wird beachtet)
				if(fehlerMarkieren(feld, schutz, x, y, 0) == 0){
					// Es gab einen Fehler in den gelockten Feldern, also wird
					// jetzt der Schreibschutz ignoriert:
					fehlerMarkieren(feld, schutz, x, y, 1);
				}
			}
		}
	}

	// Alle nicht gelockten Felder prüfen
	// (jetzt sind auf jeden Fall die gelockten Felder alle korrekt):
	for (x = 0; x < BREITE; x++) {
		for (y = 0; y < HOEHE; y++) {
			if (schutz[x][y] <= 0 && feld[x][y] > 0
					&& pruefePos(feld, x, y, feld[x][y])) {
				// Fehler gefunden
				f++;
				fehlerMarkieren(feld, schutz, x, y, 0);
			}
		}
	}

	return f;
}

int testSudokuFormalALT(int feld[BREITE][HOEHE]) {
	/* Aufbau von feld: feld[x][y], x sind spalten, y sind zeilen*/

	/* Fehlerzähler zurücksetzen*/
	rh_fehlerZaehler(6, -rh_fehlerZaehler(6, 0));
	rh_fehlerZaehler(7, -rh_fehlerZaehler(7, 0));
	rh_fehlerZaehler(8, -rh_fehlerZaehler(8, 0));

	/* Variablen (akt = aktuelle oder aktiv) */
	int zaehler[MAX_ZAHL + 1];
	int aktZeile = 0, aktSpalte = 0, aktWert = 0, kachelFeld = 0, kachelX = 0,
			kachelY = 0;

	/* Prüfe die Anzahl an gleichen Werten je Zeile (von oben nach unten) */
	for (aktZeile = 0; aktZeile < HOEHE; aktZeile++) {
		rh_resetZaehler(zaehler);
		for (aktSpalte = 0; aktSpalte < BREITE; aktSpalte++) {
			if (!rh_inkrementZaehler(zaehler, feld[aktSpalte][aktZeile])) {
				printf("Validator: Unerwartes Zeichen in Zeile %d Spalte %d.\n",
						aktZeile, aktSpalte);
			}
			for (aktWert = 1; aktWert <= MAX_ZAHL; aktWert++)
				if (zaehler[aktWert] > 1) {
					rh_fehlerZaehler(6, 1);
					if (!schutz[aktSpalte][aktZeile]) {
						schutz[aktSpalte][aktZeile] -= 1;
					}
				}
		}
	}

	/* Prüfe die Anzahl an gleichen Werten je Spalte (von links nach rechts) */
	for (aktSpalte = 0; aktSpalte < BREITE; aktSpalte++) {
		rh_resetZaehler(zaehler);
		for (aktZeile = 0; aktZeile < HOEHE; aktZeile++) {
			rh_inkrementZaehler(zaehler, feld[aktSpalte][aktZeile]);
			for (aktWert = 1; aktWert <= MAX_ZAHL; aktWert++) {
				if (zaehler[aktWert] > 1) {
					rh_fehlerZaehler(7, 1);
					if (!schutz[aktSpalte][aktZeile]) {
						schutz[aktSpalte][aktZeile] -= 1;
					}
				}
			}
		}
	}

	/* Prüfe die Anzahl an gleichen Werten je Kachel (links nach rechts und oben nach unten) */
	for (kachelY = 0; kachelY < KACHELHOEHE; kachelY++)
		for (kachelX = 0; kachelX < KACHELBREITE; kachelX++) {
			rh_resetZaehler(zaehler);
			for (kachelFeld = 0; kachelFeld < KACHELBREITE * KACHELHOEHE;
					kachelFeld++) {
				aktWert = feld[kachelFeld % KACHELBREITE
						+ kachelX * KACHELBREITE][kachelFeld / KACHELHOEHE
						+ kachelY * KACHELHOEHE];
				rh_inkrementZaehler(zaehler, aktWert);
			}
			for (aktWert = 1; aktWert <= MAX_ZAHL; aktWert++)
				if (zaehler[aktWert] > 1) {
					rh_fehlerZaehler(8, 1);
				}
		}
	//printf("Fehlerzaehler=%d\n", rh_fehlerZaehler(6,0)+rh_fehlerZaehler(7,0)+ rh_fehlerZaehler(8,0));
	return rh_fehlerZaehler(6, 0) + rh_fehlerZaehler(7, 0)
			+ rh_fehlerZaehler(8, 0);
}

/* 
 * readFile(char *pfad)
 * Liest eine Datei ein, die ein Sudoku-Rätsel enthalten sollte.
 * Ruft Funktionen auf, die die Arbeit machen.
 * Getestet mit Textdateien in utf8-Codierung auf Kubuntu 14.04LTS
 * pfad - Pfad zur Datei
 * returns  0 - Wenn keine Fehler aufgetreten sind.
 * returns -1 - Wenn Fehler auftraten, die nicht korrigiert werden konnten oder sollten.
 * returns -2 - Wenn der Dateihandle nicht geöffnet werden konnte.
 * returns  n - Anzahl der Fehler (n), die korrigiert wurden.
 */
int leseDatei(char *dateipfad) {
	/* Dateihandle öffnen */
	FILE *ptr_datei;
	ptr_datei = fopen(dateipfad, "r");
	if (!ptr_datei)
		return -2;

	/* Datei in array einlesen */
	int einleseFeld[BREITE][HOEHE];
	int einleseStatus = rh_leseDateiZeichenweise(einleseFeld, ptr_datei);
	fclose(ptr_datei);

	if (einleseStatus != 0) {
		return einleseStatus;
	}

	/* Eingabe validieren */
	int validierungsStatus = testSudokuFormal(einleseFeld);
	if (validierungsStatus) {
		// Es gibt formale Fehler
	}

	/* Feld übertragen */
	// Fragen, ob das ok ist, wenn Fehler korrigiert wurden
	// Array übertragen (echte Kopie, nicht über Pointer)
	int i, j;
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			schutz[i][j] = 0;
		}
	}
	for (i = 0; i < BREITE; i++) {
		for (j = 0; j < HOEHE; j++) {
			setFeld(i, j, einleseFeld[j][i], einleseFeld[j][i]);
		}
	}
	return einleseStatus;
}

