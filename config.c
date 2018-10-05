#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define EXTERN
#define BUFSIZE 1024
#include "./globale.h"

void oeffneConfigDatei(char *dateiname){ //öffnet eine Config-Datei, bekommt Dateinamen übergeben
	char name[256];
	strcpy(name, dateiname);
	FILE *datei;
	datei=fopen(name,"r");
  if (datei == NULL) { //prüft ob übergebener Dateiname gültig = Datei lässt sich öffnen
  
	/* Datei lässt sich nicht öffnen - Default wird verwendet */
    char defaultname[256] = "./client.conf";
	strcpy(name, defaultname);
	printf("Default Datei wird verwendet %s\n", name);
		if((datei=fopen(name,"r")) == NULL) { 
		error("Fehler beim Öffnen der Default Datei"); //falls auch Default nicht funktioniert
		}
		else{
		printf("Default geöffnet\n");
		leseConfig(datei);
		fclose(datei);
		}
 	}
	else{
		printf("Übergebene Datei %s geöffnet\n", name);
		leseConfig(datei);
		fclose(datei);
		}
}

void leseConfig(FILE *file){
	
/*
Verarbeitet die geöffnete Config-Datei
und speichert die ausgelesenen Daten im Struct "conf".
Das Struct conf wurde global in "globale.h" erstellt  
*/

	char zeile[256];
	while(fgets(zeile,256,file)){ //liest zeilenweise die Config-Datei
	
	/*Es folgt die jeweilige Abfrage nach Parametername */
	
		if(strncmp(zeile, "hostname",7) == 0){
			strcpy(zeile, split(zeile, 3));
			/* Folgende Funktion dient zum Kürzen des Hostname-Strings - dieser war zu lang
			ggf. kann diese in "editStrings / Split" implementiert werden, falls wir sie öfter benötigen */
				int dot = 0;
				while( zeile[dot] != '\0' )
				dot++;
				if(zeile[dot-1] == '\r'){
				zeile[dot-1] = '\0'; 
				}
		
			strcpy(globalconf.hostname, zeile);
	/*Aus jeder Zeile wird der Text an dritter Stelle ausgeschnitten und im Struct gespeichert */
		}
		
		if(strncmp(zeile, "portnummer",9) == 0){
			strcpy(zeile, split(zeile, 3));
			globalconf.portnummer = strtol(zeile, NULL, 10);
		}
		if(strncmp(zeile, "spielart",7) == 0){
			strcpy(zeile, split(zeile, 3));
			strcpy(globalconf.spielart, zeile);
		}		
	}
	printf("Config wurde gelesen\n");
	/* Fehlerbehandlung falls ein Parameter = NULL ? */
}