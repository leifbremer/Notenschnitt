#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get.h"


tModul* CSVDateiEinlesenSpeichernAlstModul(int* Anzahl) {

	tModul* einlesen = NULL;		//Pointer auf Moduldatentyp
	int i = 0;						//Laufvariable		
	char eingabeDatName[] = "module.csv";	//Name der Datei
	

	einlesen = calloc(i, sizeof(tModul));			//Reservieren von Speicher für ein Modul
	if (einlesen == NULL){
		printf("Nicht genug Speicher vorhanden.\n");		//Fehler wenn kein Speicher
		return 0;
	}

	FILE* eingabeDat = fopen(eingabeDatName, "r");			//Initialisirung des Pointers für die einzulsende Datei

	if (!eingabeDat){
		printf("Die auszulesene Datei konnte nicht geöffnet werden.\n");
		return 0;											//Fehler wenn Datei nicht geöffnet
	}

	char inpString[sizeof(tModul)+4];						//String mit Größe des Datentyps zum einlesen
	char* verarbeiten;										//Pointer auf char um den Aufgeteilten String zwischzuspeichern
	tModul* zwischenspeicher;								//Zwischenspeicher um bei zu wenig Speicher den NullPointer zu erhalten und das Programm zu beenden
	fgets(inpString, sizeof(tModul), eingabeDat);			//Funktionsaufruf um die erste zeile zu ignorieren

	while (fgets(inpString, sizeof(tModul), eingabeDat)){				//Solange wie noch weitere Zeilen eingelesen werden
		zwischenspeicher = realloc(einlesen, (long)(i+1) * sizeof(tModul));		//Bei jedem durchlauf den reservierten Speicher um die Größe von sModul erhöhen
		if (!zwischenspeicher){
			printf("Nicht genug Speicher vorhanden.\n");				//Fehlerausgabe falls nicht genug Speicher vorhanden
			fclose(eingabeDat);
			free(einlesen);
			return 0;
		}
		einlesen = zwischenspeicher;									//da Zwischenspeicher kein NUllPointer einlesen mit zwischenspeicher überschreiben

		verarbeiten = strtok(inpString, ";");							//Modulname trennen und abspeichern
		strcpy(einlesen[i].ModulName, verarbeiten);

		verarbeiten = strtok(NULL, ";");								//Kürzel trennen und abspeichern
		strcpy(einlesen[i].Kuerzel, verarbeiten);

		verarbeiten = strtok(NULL, ";");								//Gewichtung trennen und abspeichern
		einlesen[i].Gewichtung = atoi(verarbeiten);

		verarbeiten = strtok(NULL, ";");								//Note trennen und abspeichern
		einlesen[i].Note = atoi(verarbeiten);

		i++;															//i + 1 damit bei der nächsten eingabe der Speicher erweitert wird
	}
	fclose(eingabeDat);													//nach dem einlesen die Datei schließen
	printf("Ihre Datei wurde erfolgreich eingelesen, es wurden %d Module erkannt.\n\n", i);

	*Anzahl = i;

	return einlesen;
}

void AusgabeModule(tModul* Module, int Anzahl) {
	for (int i = 0; i < (Anzahl); i++)					//teile die Größe von Module durch die Größe von Datentyp tModul
	{
		printf("%-70s %-7s %5d %5d\n\n", Module[i].ModulName, Module[i].Kuerzel, Module[i].Gewichtung, Module[i].Note);		//Ausgabe jedes einzelnen Daten Punkts
	}
}

short getShort(	char text[])		// [in] question text for user input
{
	short value;		// user input
	int finished = 0;		// flag for correct user input
	char ch;			// character behind number
	int retVal;			// return value of scanf

	do {
		finished = 0;	// set flag incorrect
		// get user input
		printf("Bitte geben Sie Ihre Note fuer %s ein: ", text);
		ch = '\0';
		retVal = scanf("%hd%c", &value, &ch);

		// check for valid user input
		if (retVal != 2) printf("Das war keine korrekte Zahl!\n");
		else if (ch != '\n') printf("Unerwartete Zeichen hinter der Zahl!\n");
		else if ((value < 0) || (value > 15)) printf("Die Eingabe ist nicht im erwarteten Bereich\n");
		else finished = 1;

		// clear input stream
		while (ch != '\n') retVal = scanf("%c", &ch);

		// repeat if not finished
	} while (!finished);

	// return user input
	return value;
}

void NotenAbfrage(tModul* Module, int Anzahl) {
	
	for (int i = 0; i < Anzahl; i++)							
	{
		system(CLEAR);										
		printf("(%d/%d) ", (i + 1), Anzahl);				
		Module[i].Note = getShort(Module[i].ModulName);
	}
}

float BerechnungAbschlussnote(tModul* Module, int Anzahl) {

	float Gesamt = 0;
	float GewichtungGesamt = 0;
	float Schnitt = 0;
	char WortNote[16];

	for (int i = 0; i < Anzahl; i++) {
		Gesamt = Gesamt + (Module[i].Gewichtung * Module[i].Note);
		GewichtungGesamt = GewichtungGesamt + Module[i].Gewichtung;
	}

	Schnitt = Gesamt / GewichtungGesamt;
	if (Schnitt >= 12.5) strcpy(WortNote, "sehr gut");
	else if (Schnitt >= 9.5) strcpy(WortNote, "gut");
	else if (Schnitt >= 6.5) strcpy(WortNote, "befriedigend");
	else if (Schnitt >= 5) strcpy(WortNote, "ausreichend");
	else strcpy(WortNote, "nicht bestanden");


	printf("Ihre Abschlussnote ist %s mit %.2f Punkten.\n", WortNote, Schnitt);

	return Schnitt;
}

void CSVDateiAusgabe(tModul* Module, int Anzahl, float Abschlussnote) {

	FILE* CSVout = fopen("ModuleNoten.CSV", "w");
	if (!CSVout){
		printf("Die Datei konnte nicht geöffnet werden");

	}else{
		for (int i = 0; i < Anzahl; i++){
			fprintf(CSVout, "%s;%s;%d;%d\n", Module[i].ModulName, Module[i].Kuerzel, Module[i].Gewichtung, Module[i].Note);
		}
		fprintf(CSVout, "\n\n Damit haben sie eine Gesamtnote von: %f", Abschlussnote);
		fclose(CSVout);

		printf("Ihre Datei wurde erfolgreich erstellt.");
	}
}


void printInterface() {

	printf("Was moechten Sie tun?\n\n");
	printf("\"einlesen\" fuer Datei einlesen\n");
	printf("\"ausgabe\" fuer die Ausgabe der Module in der Konsole\n");
	printf("\"noten\" um Ihre Noten einzugeben\n");
	printf("\"schnitt\" fuer die Gesamtnote\n");
	printf("\"drucken\" um die Daten in einer Datei auszugeben\n");
	printf("\"ende\" um das Programm zu beenden\n\n");
}
