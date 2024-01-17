#ifndef Funktionen1

#define Funktionen1

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

typedef struct sModul
{											//Definition von Struktur für Module
	char ModulName[100];
	char Kuerzel[6];
	short Gewichtung;
	short Note;
} tModul;

tModul* CSVDateiEinlesenSpeichernAlstModul(int* Anzahl);		//Funktion zum Einlesen einer CSV Datei
void AusgabeModule(tModul* Module, int Anzahl);					//Ausgabe der einzelnen Module
short getShort(char text[]);									//Sichere Abfrage vom Typ Short
void NotenAbfrage(tModul* Module, int Anzahl);					//EInzelne Abfrage der Notenpunkte
float BerechnungAbschlussnote(tModul* Module, int Anzahl);		//Berechnung Der Abschlussnote
void CSVDateiAusgabe(tModul* Module, int Anzahl, float Abschlussnote);	//Funktion zur Speicherung der Daten in einer CSV Datei
void printInterface();											//Userinterface Ausgabe 


#endif
