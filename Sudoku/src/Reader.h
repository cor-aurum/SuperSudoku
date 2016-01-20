/******************************************************************************
 * SUDOKU ./reader.c                                                          *
 * Einlesefunktion für Textdateien mit Sudoku Rätzeln gemäß der Vorgaben      *
 * Ungetestet nicht  produktionsreif, Fehlerkorrektur noch verbessbar         *
 * Version 1 (2015-12-28) Sascha Scherrer <dhbw.scherrer@gmail.com>           *
 ******************************************************************************/

/*
 #include<stdio.h>
 #include<stdlib.h>
 #define HOEHE 9
 #define BREITE 9
 #define KACHELHOEHE 3
 #define KACHELBREITE 3
 #define MAX_ZAHL 9
 */

int pruefe(int feld[BREITE][HOEHE], int x, int y, int zaehler);


/* HILFSFUNKTIONEN (Präfix 'rh_' für reader heĺper) ***************************/

/*
 * Hilfsfunktion rh_fehlerZaehler(int typ, anzahlNeueFehler)
 * Zählt die Anzahl an Fehlern, die während des Einlesens gefunden werden.
 * return -1 - wenn der Typ nicht im zugelassenen Wertebereich ist.
 * return n - die aktuelle anzahl an Fehlern des angegebenen Typ
 */
int rh_fehlerZaehler(int typ, int anzahlNeueFehler) {
	static int anzahlFehler[] = { 0, 0, 0, 0, 0, 0, 0, 0 ,0};
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
 * Hilfsfunktion rh_dateipfadTesten(char *dateipfad)
 * Testet, ob der Dateipfad existiert
 * TODO: Test implementieren
 * returns  0 - wenn der Dateipfad in Ordnung ist.
 * returns -1 - wenn etwas nicht stimmt.
 */
int rh_dateipfadTester(char *dateipfad) {
	return 0;
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
 * - Leerzeilen werden ignoriert (TODO: Bekommen wir das besser hin?)
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
				/* TODO: Sollen wir die Berücksichtigung von Leerzeichen hier herausnehmen?    */
				rh_fehlerZaehler(1, 1); // corrigierteFehler++
				rh_fehlerZaehler(2, 1); // irrelevanteZeilen++
				printf(
						"Datei: Zeile %d enthällt keine gültigen Zeichen und wird ignoriert.\n",
						posY);
			} else {
				if (posX != BREITE) {
					printf(
							"Datei: Zeile %d enthällt nicht die erwartete Zahl an Zeichen."
									"Zeichen werden ausgelassen oder ergänzt.\n",
							posY);
					rh_fehlerZaehler(1, 1); // corrigierteFehler++
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
			rh_fehlerZaehler(1, 1); // corrigierteFehler++
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
int pruefePos(int feld[BREITE][HOEHE], int x, int y, int wert)
{
	int fehler = 0, aktX, aktY;

	// In Zeile
	for(aktX = 0; aktX < BREITE; aktX++)
		if(feld[aktX][y] == wert && aktX != x)
			fehler++;

	// In Spalte
	for(aktY = 0; aktY < BREITE; aktY++)
		if(feld[x][aktY] == wert && aktY != y)
			fehler++;

	// In Kachel
	for(aktX = (x % KACHELBREITE) * KACHELBREITE; aktX < (x % KACHELBREITE +1 ) * KACHELBREITE; aktX++)
		for(aktY = (y % KACHELHOEHE) * KACHELHOEHE; aktY < (y % KACHELHOEHE +1 ) * KACHELHOEHE; aktY++)
			if(feld[aktX, aktY] == wert && aktX != x && aktY != y)
				fehler++;

	return fehler;
}



/* HAUPTFUNKTIONEN ************************************************************/

/*
 * testSudokuFormal(int feld[][])
 * Prüft das Sudoku auf Formale korrektheit und setzt fehlermarkierungen in schutz.
 * returns 0 - wenn das feld formal korrekt ist
 * returns n - die Anzahl der Fehler, die gefunden wurden. 
 */
int testSudokuFormal(int feld[BREITE][HOEHE])
{
	int x=0, y=0, f=0;
	int istFalsch[BREITE][HOEHE];
	int fehlerJeWert[MAX_ZAHL+1];

	// Fehlerspeicher zurücksetzen (Schutz > 0 ist gelockt, Schutz < 0 ist Fehler)
	for(y=0; y<HOEHE; y++){
		for(x=0; x<BREITE; x++){
			if(schutz[x][y] < 0)
				schutz[x][y] = 0;
		}
	}
	rh_resetArray1D(fehlerJeWert, MAX_ZAHL+1, 0);

	// Fehler finden
	for(y=0; y<HOEHE; y++){
		for(x=0; x<BREITE; x++){
			if(feld[x][y] > 0 && pruefePos(feld,x,y,feld[x][y])){
				// Fehler zaehlen
				f++;
				fehlerJeWert[(feld[x][y] > MAX_ZAHL ? 0 : feld[x][y])]++;
				// Gelockte Felder mit negativem VorzeichMn speichern
				istFalsch[x][y] = schutz[x][y] > 0 ? -feld[x][y] : feld[x][y];
			}
		}
	}

	// Schauen, was markiert und was freigegeben werden muss.
	int gelockte, wert;
	for(wert = 1; wert <= MAX_ZAHL; wert++)
		if(fehlerJeWert[wert]){
			// Für alle Werte bei denen es fehler gab:

			// Wie viele geblockte gibt es?
			gelockte = 0;
			for(x=0; x < BREITE; x++)
				for(y=0; y < HOEHE; y++)
					if(istFalsch[x][y] == -wert)
						gelockte++;

			// TODO: Hier sollte man noch prüfen, ob es Probleme innerhalb der gelockten Felder gibt.
			// Wenn es weniger gelockte Felder als Fehler gab: Alle nicht gelockten als falsch markieren
			if(gelockte < fehlerJeWert[wert]){
				for(x=0; x < BREITE; x++)
					for(y=0; y < HOEHE; y++)
						if(istFalsch[x][y] == wert)
							schutz[x][y] = -wert;
			}else{
			// Andernfalls: Auch gelockte Felder als falsch markieren und deren lock aufheben.
				for(x=0; x < BREITE; x++)
					for(y=0; y < HOEHE; y++)
						if(istFalsch[x][y] == wert || istFalsch[x][y] == -wert)
							schutz[x][y] = wert > 0 ? -wert : wert;
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
	//printf("Fehlerzaehler zurückgesetzt.\n");

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

#ifdef NO
			printf(
					"Validator: Wert %d kommt in Zeile %d %d mal vor. (%d mal zuviel).\n",
					aktWert, aktZeile + 1, zaehler[aktWert],
					zaehler[aktWert] - 1);
#endif
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
#ifdef NO
			printf(
					"Validator: Wert %d kommt in der Spalte %d %d mal vor (%d mal zuviel).\n",
					aktWert, aktZeile + 1, zaehler[aktWert],
					zaehler[aktWert] - 1);
#endif
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
#ifdef NO
					printf(
							"Validator: Wert %d kommt in Teilfeld zwischen Zeile %d Spalte %d \n"
							"           und Zeile %d Spalte %d %d mal vor (%d mal zuviel).\n",
							aktWert, kachelY * KACHELHOEHE,
							kachelX * KACHELBREITE,
							(kachelY + 1) * KACHELHOEHE - 1,
							(kachelX + 1) * KACHELHOEHE - 1, zaehler[aktWert],
							zaehler[aktWert - 1]);
#endif
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
 * TODO: Teste mit anderen Dateien
 * TODO: Teste auf anderen Betriebssystemen (insbesondere Windows)
 * TODO: Teste mit MinGW gcc
 * pfad - Pfad zur Datei
 * returns  0 - Wenn keine Fehler aufgetreten sind.
 * returns -1 - Wenn Fehler auftraten, die nicht korrigiert werden konnten oder sollten.
 * returns -2 - Wenn die Datei nicht gefunden wurde oder der Pfad nicht valide ist.
 * returns -3 - Wenn die Datei keine Textdatei ist. TODO: Wie kann man das testen?
 * returns -4 - Wenn der Dateihandle nicht geöffnet werden konnte.
 * returns  n - Anzahl der Fehler (n), die korrigiert wurden.
 */
int leseDatei(char *dateipfad) {
	/* Dateipfad prüfen */
	if (rh_dateipfadTester(dateipfad))
		return -2;

	/* Dateihandle öffnen */
	FILE *ptr_datei;
	ptr_datei = fopen(dateipfad, "r");
	if (!ptr_datei)
		return -4;

	/* Datei in array einlesen */
	int einleseFeld[BREITE][HOEHE];
	int einleseStatus = rh_leseDateiZeichenweise(einleseFeld, ptr_datei);
	fclose(ptr_datei);
	if (einleseStatus) {
		printf(
				"Es traten Fehler beim einlesen der Datei auf. Fortfahren? (y/n) ");
		printf("y\n");
	}

	/* Eingabe validieren */
	int validierungsStatus = testSudokuFormal(einleseFeld);
	if (validierungsStatus) {
		printf(
				"Das Sudoku beinhaltet formale Fehler. Dennoch fortfahren? (y/n) ");
		printf("y\n");
	}
	if (einleseStatus != 0) {
		return einleseStatus;
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

