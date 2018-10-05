#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define EXTERN
#define BUFSIZE 1024
#include "./globale.h"

//Print der aktuellen gespeicherten Steine und deren Wert
void printArray(){
			printf("STEINE: 0 0 : %d\n", shm_connectPiecelist->steine[0][0]);
			printf("STEINE: 0 1 : %d\n", shm_connectPiecelist->steine[0][1]);
			printf("STEINE: 0 2 : %d\n", shm_connectPiecelist->steine[0][2]);
			printf("STEINE: 0 3 : %d\n", shm_connectPiecelist->steine[0][3]);
			printf("STEINE: 0 4 : %d\n", shm_connectPiecelist->steine[0][4]);
			printf("STEINE: 0 5 : %d\n", shm_connectPiecelist->steine[0][5]);
			printf("STEINE: 0 6 : %d\n", shm_connectPiecelist->steine[0][6]);
			printf("STEINE: 0 7 : %d\n", shm_connectPiecelist->steine[0][7]);
			printf("STEINE: 1 0 : %d\n", shm_connectPiecelist->steine[1][0]);
			printf("STEINE: 1 1 : %d\n", shm_connectPiecelist->steine[1][1]);
			printf("STEINE: 1 2 : %d\n", shm_connectPiecelist->steine[1][2]);
			printf("STEINE: 1 3 : %d\n", shm_connectPiecelist->steine[1][3]);
			printf("STEINE: 1 4 : %d\n", shm_connectPiecelist->steine[1][4]);
			printf("STEINE: 1 5 : %d\n", shm_connectPiecelist->steine[1][5]);
			printf("STEINE: 1 6 : %d\n", shm_connectPiecelist->steine[1][6]);
			printf("STEINE: 1 7 : %d\n", shm_connectPiecelist->steine[1][7]);
			printf("STEINE: 2 0 : %d\n", shm_connectPiecelist->steine[2][0]);
			printf("STEINE: 2 1 : %d\n", shm_connectPiecelist->steine[2][1]);
			printf("STEINE: 2 2 : %d\n", shm_connectPiecelist->steine[2][2]);
			printf("STEINE: 2 3 : %d\n", shm_connectPiecelist->steine[2][3]);
			printf("STEINE: 2 4 : %d\n", shm_connectPiecelist->steine[2][4]);
			printf("STEINE: 2 5 : %d\n", shm_connectPiecelist->steine[2][5]);
			printf("STEINE: 2 6 : %d\n", shm_connectPiecelist->steine[2][6]);
			printf("STEINE: 2 7 : %d\n", shm_connectPiecelist->steine[2][7]);
}

//Funktion, welche das Wort an der position aus einem String ausgibt
char *split(char *string, int position){
	
    char str1[BUFSIZE];
    strcpy(str1, string);
    int i,j,ctr;
    int stringlength = strlen(str1);    
 
    j=0; ctr=0;
    for(i=0;i<=stringlength;i++)
    {
        // if space or NULL found, assign NULL into newString[ctr]
        if(str1[i]==' '||str1[i]=='\0'||str1[i]=='\n')
        {	
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=str1[i];
            j++;
        }		
		
    }
    i = position-1;
		
    return newString[i];
}

//Teilt den übergebenen Server-String und schickt jeweils das gefundene "Piece" an "speicherPieceStart"
void serverStringTeilen(char *string){
	
	int i;
	int los;
	int piecenr; //beschreibt die Anzahl der gefundenen "Piecelist"-Teilstrings
	i=0;
	piecenr = 0;
	los = 0;
	char str1[BUFSIZE];
	char str2[BUFSIZE];
    strcpy(str1, string);
	strcpy(str2, str1);
	int stringlength = strlen(str1); 
	char teil[BUFSIZE];

	//Resettet die gespeicherten PieceList Angaben
	for(int nulli = 0; nulli < 3; nulli++){
		for(int nullj = 0; nullj < 8; nullj++){
			shm_connectPiecelist->steine[nulli][nullj] = 0;
		}
	}
	shm_connectPiecelist->anzahlGeschlageneSteine = 0;

	while(i<=stringlength){
		strcpy(teil, split(str2, i));
		if(strncmp(split(str2, i), "CAPTURE", 7) == 0){
			los = 1;
			i++;
			strcpy(teil, split(str2, i));
			shm_connectPiecelist->capture = (int) strtol(teil, NULL, 10);
			i += 4;
			strcpy(teil, split(str2, i));
		}
		if (strncmp(teil, "ENDPIECE", 8) == 0){ 
			// Prüft auf Ende der Piecelist
			break;
		} 
		if (los == 1){
		if (strncmp(teil, "+", 1) != 0 && strncmp(teil,"PIE", 3) != 0){
			//printf("Piece gefunden %s\n", teil);
			
			//Weiterleitung des aktuellen Piecelist- Strings an die nächste Funktion
			speicherPieceStart(teil, piecenr);
			piecenr++;
			}
		}
		i++;
	}
	printArray();
}

//Speichert die Informationen im übergebenen Teilsstring (z.B. A5) in den Piecelist-SHM
void speicherPieceStart(char *string, int piecenr){
	char stringakt[BUFSIZE];
	strcpy(stringakt, string);
	int nr, w, j, unr; // w ist Spieler 1 oder 2 also Wert 1 oder 5
	nr = piecenr;
	unr = 0;
	w = 0;
	j = 20;
	
	if(shm_connect->unsereSpielerNummer == 0){ //Unsere Steine sind in der Piecelist vom Server die ersten 9
		if (nr < 9){ //Alle Steine, die bis Stein 9 genannt werden sind unsere --> Wert 1
			w = 1;
			unr = 1;
			} 
		else {
			w = 5;
			} 
	}
	
	else if (shm_connect->unsereSpielerNummer == 1) { //Unsere Steine sind in der Piecelist vom Server die zweiten 9
		
		if (nr > 8){
			w = 1; //Alle Steine, die ab Stein 9 genannt werden sind unsere --> Wert 1
			unr = 2;
		} 
		else {
			w = 5;
		} 
	}
	printf("W: %i\n", w);
	j = (int)(stringakt[1]-'0'); //Wandelt die zweite Position des übergebenen Strings in int um und speichert es in j (A5 --> j = 5)
	
		if(j < 0 || j > 8){
			printf("Stein nicht gesetzt\n"); //kein int Wert gefunden (nur A oder C übergeben)
			if(strncmp(stringakt, "C", 1) == 0){
				if((unr == 1) && (nr < 9)){
				shm_connectPiecelist->anzahlGeschlageneSteine++;
				}
				else if ((unr == 2) && (nr > 8)){
				shm_connectPiecelist->anzahlGeschlageneSteine++;
				}
			}
			printf("Aktuell haben wir nur noch 9 - %i Steine auf dem Feld\n", shm_connectPiecelist->anzahlGeschlageneSteine);
		}
		
		//speichert je nach erstem Buchstaben in 0/1/2
		else if(strncmp(stringakt, "A", 1) == 0){
			shm_connectPiecelist->steine[0][j] = w;
		}
		else if (strncmp(stringakt, "B", 1) == 0){
			shm_connectPiecelist->steine[1][j] = w;
		}
		else if (strncmp(stringakt, "C", 1) == 0){
			shm_connectPiecelist->steine[2][j] = w;
		}
}
