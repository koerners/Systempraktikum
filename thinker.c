#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define BUFSIZE 1024
#define EXTERN 
#include "./globale.h"

char koord[5];
int counterGesetzt = 0;
int springenErlaubt = 0;
char buf[BUFSIZE];
char zug[BUFSIZE];
int k,l;
int ca;
int mmP1 = 0; //i Feld, das frei ist
int mmP2 = 0; //j Feld, das frei ist
int millS1i = 0;
int millS1j = 0;
int millS2i = 0;
int millS2j = 0;


int mmP1A[18];
int mmP2A[18];
int millS1iA[18];
int millS1jA[18];
int millS2iA[18];
int millS2jA[18];

int mmP1V[18];
int mmP2V[18];
int millS1iV[18];
int millS1jV[18];
int millS2iV[18];
int millS2jV[18];

int checkMillClosed(int steinWert){
	
int steinW = steinWert;
int geschlosseneMill = 0;
int mmtest;

if(steinW == 3){
	
//Mühlen je A/B/C abfragen
for (int mmi = 0; mmi < 3; mmi++)
	{	
		int mmj2 = 1;
		int mmj3 = 2;
		
			for(int mmj1 = 0; mmj1 < 7; mmj1 += 2){
				mmtest = 0;
				if(mmj3 == 8){mmj3 = 0;}
				mmtest = shm_connectPiecelist->steine[mmi][mmj1] + shm_connectPiecelist->steine[mmi][mmj2] + shm_connectPiecelist->steine[mmi][mmj3];
				printf("Mmtest: %i\n", mmtest);
					if (mmtest == steinW){
						printf("Wir haben eine geschlossene Mühle\n");

							mmP1V[geschlosseneMill] = mmi;
							mmP2V[geschlosseneMill] = mmj1;
							millS1iV[geschlosseneMill] = mmi;
							millS1jV[geschlosseneMill] = mmj2;
							millS2iV[geschlosseneMill] = mmi;
							millS2jV[geschlosseneMill] = mmj3;
							geschlosseneMill++;
					}
			}
				mmj2 += 2;
				mmj3 += 2;
	}

//Mühlen je auf 1/3/5/7	abfragen
for (int mmj = 1; mmj < 8; mmj += 2){
		mmtest = 0;
		mmtest = shm_connectPiecelist->steine[0][mmj] + shm_connectPiecelist->steine[1][mmj] + shm_connectPiecelist->steine[2][mmj];
			if (mmtest == steinW){
				printf("Wir haben eine geschlossene Mühle - gleiche Zahl\n");
					mmP1V[geschlosseneMill] = 0;
					mmP2V[geschlosseneMill] = mmj;
					millS1iV[geschlosseneMill] = 1;
					millS1jV[geschlosseneMill] = mmj;
					millS2iV[geschlosseneMill] = 2;
					millS2jV[geschlosseneMill] = mmj;
					geschlosseneMill++;
				}
		}
	}
return geschlosseneMill;	
}

int checkMillAlle(int steinWert){
	printf("Willkommen bei CheckMillAlle!\n");
	int mmtest = 0;
	int myMill = 0;
	int steinW = steinWert;

	//Mühlen je A/B/C abfragen
for (int mmi = 0; mmi < 3; mmi++)
	{	
		int mmj2 = 1;
		int mmj3 = 2;
		
			for(int mmj1 = 0; mmj1 < 7; mmj1 += 2){
				mmtest = 0;
				if(mmj3 == 8){mmj3 = 0;}
				mmtest = shm_connectPiecelist->steine[mmi][mmj1] + shm_connectPiecelist->steine[mmi][mmj2] + shm_connectPiecelist->steine[mmi][mmj3];
				printf("Mmtest: %i\n", mmtest);
					if (mmtest == steinW){
						printf("Mühle kann geschlossen werden!\n");
						//checken welches Feld frei ist
						if (shm_connectPiecelist->steine[mmi][mmj1] == 0){
							mmP1A[myMill] = mmi;
							mmP2A[myMill] = mmj1;
							millS1iA[myMill] = mmi;
							millS1jA[myMill] = mmj2;
							millS2iA[myMill] = mmi;
							millS2jA[myMill] = mmj3;
							myMill++;
						}
						else if(shm_connectPiecelist->steine[mmi][mmj2] == 0){
							mmP1A[myMill] = mmi;
							mmP2A[myMill] = mmj2;
							millS1iA[myMill] = mmi;
							millS1jA[myMill] = mmj1;
							millS2iA[myMill] = mmi;
							millS2jA[myMill] = mmj3;
							myMill++;
						}
						else if(shm_connectPiecelist->steine[mmi][mmj3] == 0){
							mmP1A[myMill] = mmi;
							mmP2A[myMill] = mmj3;
							millS1iA[myMill] = mmi;
							millS1jA[myMill] = mmj1;
							millS2iA[myMill] = mmi;
							millS2jA[myMill] = mmj2;
							myMill++;
						}
					}
				mmj2 += 2;
				mmj3 += 2;
			}
	}

//Mühlen je auf 1/3/5/7	abfragen
for (int mmj = 1; mmj < 8; mmj += 2){
		mmtest = 0;
		mmtest = shm_connectPiecelist->steine[0][mmj] + shm_connectPiecelist->steine[1][mmj] + shm_connectPiecelist->steine[2][mmj];
			if (mmtest == steinW){
				printf("Mühle über ABC kann geschlossen werden!\n");
				//checken welches Feld frei ist
				if (shm_connectPiecelist->steine[0][mmj] == 0){
					mmP1A[myMill] = 0;
					mmP2A[myMill] = mmj;
					millS1iA[myMill] = 1;
					millS1jA[myMill] = mmj;
					millS2iA[myMill] = 2;
					millS2jA[myMill] = mmj;
					myMill++;
				}
				else if(shm_connectPiecelist->steine[1][mmj] == 0){
					mmP1A[myMill] = 1;
					mmP2A[myMill] = mmj;
					millS1iA[myMill] = 0;
					millS1jA[myMill] = mmj;
					millS2iA[myMill] = 2;
					millS2jA[myMill] = mmj;
					myMill++;
				}
				else if(shm_connectPiecelist->steine[2][mmj] == 0){
					mmP1A[myMill] = 2;
					mmP2A[myMill] = mmj;
					millS1iA[myMill] = 0;
					millS1jA[myMill] = mmj;
					millS2iA[myMill] = 1;
					millS2jA[myMill] = mmj;
					myMill++;
				}
			}
	}
return myMill;
}

//Prüft auf möglicherweise schließbare Mühlen - je nach übergebenem Wert eigene oder fremde (2/10)
// Zweiter Wert gibt an ob nach der ersten Mühle (0) oder nach allen möglichen Mühlen (1) gesucht werden soll
//speichert letzte leere Position einer möglichen Mühle in "mmP1" und "mmP2" (Variable für thinker komplett)
int checkMill(int steinWert){
	printf("Willkommen bei CheckMill!\n");
	int mmtest = 0;
	int myMill = 0;
	int steinW = steinWert;

	//Mühlen je A/B/C abfragen
for (int mmi = 0; mmi < 3; mmi++)
	{	
		int mmj2 = 1;
		int mmj3 = 2;
		
			for(int mmj1 = 0; mmj1 < 7; mmj1 += 2){
				mmtest = 0;
				if(mmj3 == 8){mmj3 = 0;}
				mmtest = shm_connectPiecelist->steine[mmi][mmj1] + shm_connectPiecelist->steine[mmi][mmj2] + shm_connectPiecelist->steine[mmi][mmj3];
				printf("Mmtest: %i\n", mmtest);
					if (mmtest == steinW){
						printf("Mühle kann geschlossen werden!\n");
						//checken welches Feld frei ist
						if (shm_connectPiecelist->steine[mmi][mmj1] == 0){
							mmP1 = mmi;
							mmP2 = mmj1;
							millS1i = mmi;
							millS1j = mmj2;
							millS2i = mmi;
							millS2j = mmj3;
							myMill = 1;
						}
						else if(shm_connectPiecelist->steine[mmi][mmj2] == 0){
							mmP1 = mmi;
							mmP2 = mmj2;
							millS1i = mmi;
							millS1j = mmj1;
							millS2i = mmi;
							millS2j = mmj3;
							myMill = 1;
						}
						else if(shm_connectPiecelist->steine[mmi][mmj3] == 0){
							mmP1 = mmi;
							mmP2 = mmj3;
							millS1i = mmi;
							millS1j = mmj1;
							millS2i = mmi;
							millS2j = mmj2;
							myMill = 1;
						}
						
				
						else {
							error("FEHLER bei Mühlensuche");
						}
						printf("Freies Feld ist %d %d", mmP1, mmP2);
						break;
					}
				mmj2 += 2;
				mmj3 += 2;
			}
		if(myMill == 1){ break;}
	}

//Mühlen je auf 1/3/5/7	abfragen
if(myMill == 0){
	for (int mmj = 1; mmj < 8; mmj += 2){
		mmtest = 0;
		mmtest = shm_connectPiecelist->steine[0][mmj] + shm_connectPiecelist->steine[1][mmj] + shm_connectPiecelist->steine[2][mmj];
			if (mmtest == steinW){
				printf("Mühle über ABC kann geschlossen werden!\n");
				//checken welches Feld frei ist
				if (shm_connectPiecelist->steine[0][mmj] == 0){
					mmP1 = 0;
					mmP2 = mmj;
					millS1i = 1;
					millS1j = mmj;
					millS2i = 2;
					millS2j = mmj;
					myMill = 1;
				}
				else if(shm_connectPiecelist->steine[1][mmj] == 0){
					mmP1 = 1;
					mmP2 = mmj;
					millS1i = 0;
					millS1j = mmj;
					millS2i = 2;
					millS2j = mmj;
					myMill = 1;
				}
				else if(shm_connectPiecelist->steine[2][mmj] == 0){
					mmP1 = 2;
					mmP2 = mmj;
					millS1i = 0;
					millS1j = mmj;
					millS2i = 1;
					millS2j = mmj;
					myMill = 1;
				}
				
				else {
					error("FEHLER bei Mühlensuche");
				}
				printf("Freies Feld ist %d %d", mmP1, mmP2);
				break;
			}
	}
	}
return myMill;
}

//Setzt einen Steinen auf km, kj und speichert in buf
void setzte( int km , int kj){
	
	switch (km){
		case 0:
			buf[0] = 'A';
			break;
		case 1:
			buf[0] = 'B';
			break;

		case 2:
			buf[0] = 'C';
			break;

	}
	
	buf[1] = kj + '0';
	printf("****************** %s *******************\n", buf);
}

//schiebt einen stein von si, sj nach sk, sl und speichert Zug in buf
void schiebe(int si, int sj, int sk, int sl, int schiebeart){ //wenn schiebeart == 0: Schieben von si, sj auf sk, sl - sonst sk, sl aus si, sj schieben
	
	if (schiebeart == 1){
	int temp1, temp2;
	temp1 = sk;
	temp2 = sl;
	sk = si;
	sl = sj;
	si = temp1;
	sj = temp2;
	}
	switch (si){
		case 0:
			buf[0] = 'A';
			break;
		case 1:
			buf[0] = 'B';
			break;

		case 2:
			buf[0] = 'C';
			break;

	}
	
	buf[1] = sj + '0';
	buf[2] = ':';
	switch (sk){
		case 0:
			buf[3] = 'A';
			break;
		case 1:
			buf[3] = 'B';
			break;

		case 2:
			buf[3] = 'C';
			break;

	}
	buf[4] = sl + '0';
	printf("****************** %s *******************\n", buf);
	
}

/* Methode zum springen, prüft nicht auf Mühle (schließen/verhindern) */
void springe(){
int doneS = 0;

int eigenemuehle = 0;
//Prüft auf eigene und Fremdemühle
eigenemuehle = checkMill(2);
if(eigenemuehle == 1){
	printf("Eigene Mühle gefunden, Mühle wird geschlossen");
	// Springe auf die freie Stelle in der Mühle - aber nicht mit den beiden verbleibenden Mühlensteinen
	
	//fri und frj müssen den "freien" Stein repräsentieren
	for (int fri = 0; fri < 3; fri++)
	{
		for (int frj = 0; frj < 8; frj++)
		{
			if(shm_connectPiecelist->steine[fri][frj] == 1){ //suche eigenen Stein
				//Vergleiche Position des Stein mit Position der Steine in der Mühle millS1i, millS1j, millS2i, millS2j
				if((fri != millS1i || frj != millS1j) && (fri != millS2i || frj != millS2j)){
					schiebe(fri, frj, mmP1, mmP2, ca);
					doneS = 1;
					break;
				}
			} 
		}
	if (doneS == 1) break;
	}
}
	
else if(eigenemuehle == 0){
	int fremdemuehle = 0;
	fremdemuehle = checkMill(10);
		if(fremdemuehle == 1){
			//Springe mit einem Stein auf diese Stelle (Stein beliebig)
			printf("Fremde Mühle gefunden, Mühle wird verhindert");
			for (int org1 = 0; org1 < 3; org1++)
			{
				for (int org2 = 0; org2 < 8; org2++)
				{
					if(shm_connectPiecelist->steine[org1][org2] == 1){ //suche eigenen Stein - Wenn gefunden, dann springe in fremde Mühle
						schiebe(org1, org2, mmP1, mmP2, ca);
						doneS = 1;
						break;
					} 
				}
			if (doneS == 1) break;
			}
		}
	}


if(doneS == 0){
//beliebigen Stein beliebig springen
for (int org1 = 0; org1 < 3; org1++)
	{
		for (int org2 = 0; org2 < 8; org2++)
		{
			if(shm_connectPiecelist->steine[org1][org2] == 1){ //suche eigenen Stein
			//Wenn Stein gefunde, suche freies Feld zum Springen
				for (int fri = 0; fri < 3; fri++)
				{
					for (int frj = 0; frj < 8; frj++)
					{
						if(shm_connectPiecelist->steine[fri][frj] == 0){ //suche freies Feld
						//Wenn freies Feld gefunden, springe mit Stein dorthin
						schiebe(org1, org2, fri, frj, ca);
						doneS = 1;
						break;
						} 
					}
				if (doneS == 1) break;
				}
			}
		if (doneS == 1) break;
		}
	if (doneS == 1) break;
	}
}
} 

//püft ob ein Feld neben der Ecke ci,cj frei ist -> führt "schiebe" zu diesem Feld aus und 1 wenn ja
int checkCorner(int ci, int cj, int checkartC, int ausi1, int ausj1, int ausi2, int ausj2){ //m == j+1, n==j-1
	printf("checkCorner mit %d %d\n", ci, cj );
	int checkCorner = 0;
	ca = checkartC;
	int cm,cn;
	int dj = 0;
	int ej = 0;
	cm = cj+1;
	cn = cj-1;
	if(cj == 0){cn = 7;}
	else if (cj == 7){cm = 0;}

	printf("cj %d cm %d cn %d ci %d ca %d \n",cj, cm, cn, ci, ca );
	printf("ausi1 %d ausj1 %d ausi2 %d ausj2 %d\n", ausi1, ausj1, ausi2, ausj2);
	printf("shm ci cm %d\n", shm_connectPiecelist->steine[ci][cm] );

	if(shm_connectPiecelist->steine[ci][cm] == ca){
		if(!(((ci == ausi1) && (cm == ausj1)) || ((ci == ausi2) && (cm == ausj2)))){
			
			printf("ci %d cm %d, aus: %d %d %d %d\n", ci, cm, ausi1, ausj1, ausi2, ausj2);
			dj = cm;	
			printf("dj %d", dj);
			schiebe(ci, cj, ci, dj, ca);
			checkCorner = 1;
		}
	}
	
	if ((shm_connectPiecelist->steine[ci][cn] == ca) && (checkCorner != 1)){
		if(!(((ci == ausi1) && (cn == ausj1)) || ((ci == ausi2) && (cn == ausj2)))){
			
			printf("ci %d cn %d, aus: %d %d %d %d\n", ci, cn, ausi1, ausj1, ausi2, ausj2);
			ej = cn;		
			printf("ej %d", ej);
			schiebe(ci, cj, ci, ej, ca);
			checkCorner = 1;
		}
	}
return checkCorner;
}

//püft ob ein Feld auf gleicher Buchstabenreihe in +/- Richtung frei ist -> führt "schiebe" zu diesem Feld aus und 1 wenn ja
int checkPM(int pmi, int pmj, int checkartPM, int ausi1, int ausj1, int ausi2, int ausj2){ 
	printf("Wir sind in checkPM");
	int checkPM = 0;
	int pmjn, pmjn2;
	ca = checkartPM;
	pmjn = pmj+1;
	pmjn2 = pmj-1;
	if(pmj == 0){pmjn2 = 7;}
	else if (pmj == 7){pmjn = 0;}
	
	if(shm_connectPiecelist->steine[pmi][pmjn] == ca){
		if(!(((pmi == ausi1) && (pmjn == ausj1)) || ((pmi == ausi2) && (pmjn == ausj2))))
				{
				printf("pmi %d pmjn %d, aus: %d %d %d %d\n", pmi, pmjn, ausi1, ausj1, ausi2, ausj2);
				schiebe(pmi, pmj, pmi, pmjn, ca);
				checkPM = 1;
				}
	}
	if ((shm_connectPiecelist->steine[pmi][pmjn2] == ca) && (checkPM != 1)){
		if(!(((pmi == ausi1) && (pmjn2 == ausj1)) || ((pmi == ausi2) && (pmjn2 == ausj2))))
				{
				printf("pmi %d pmjn2 %d, aus: %d %d %d %d\n", pmi, pmjn2, ausi1, ausj1, ausi2, ausj2);
			schiebe(pmi, pmj, pmi, pmjn2, ca);
			checkPM = 1;
				}
	}
return checkPM;
}

//püft ob ein Feld auf gleicher Zahlenreihe frei ist -> führt "schiebe" zu diesem Feld aus und 1 wenn ja
int checkABC(int abi, int abj, int checkart, int ausi1, int ausj1, int ausi2, int ausj2){
	int checkABC = 0;
	ca = checkart;
	printf("Wir sind in checkABC");
	printf("ca = %d\n", ca);

	switch(abi){
		case 0:
			if(shm_connectPiecelist->steine[abi+1][abj] == ca){
					if(!(((abi+1 == ausi1) && (abj == ausj1)) || ((abi+1 == ausi2) && (abj == ausj2)))){
						printf("case0, KEIN Ausschluss: abi: %d, abj %d", abi, abj);
						schiebe(abi, abj, abi+1, abj, ca);
						checkABC = 1;
					}
			}
		break;
		
		case 1:
			if(shm_connectPiecelist->steine[abi+1][abj] == ca){
				if(!(((abi+1 == ausi1) && (abj == ausj1)) || ((abi+1 == ausi2) && (abj == ausj2)))){
					printf("case1, KEIN Ausschluss: abi: %d, abj %d", abi, abj);
					schiebe(abi, abj, abi+1, abj, ca);
					checkABC = 1;
					}
				}
			else if(shm_connectPiecelist->steine[abi-1][abj] == ca){
				if(!(((abi-1 == ausi1) && (abj == ausj1)) || ((abi-1 == ausi2) && (abj == ausj2)))){
					printf("case1, KEIN Ausschluss: abi: %d, abj %d", abi, abj);
					schiebe(abi, abj, abi-1, abj, ca);
					checkABC = 1;
					}
				}
		break;
		
		case 2:
			if(shm_connectPiecelist->steine[abi-1][abj] == ca){
					if(!(((abi-1 == ausi1) && (abj == ausj1)) || ((abi-1 == ausi2) && (abj == ausj2)))){
						printf("case2, KEIN Ausschluss: abi: %d, abj %d", abi, abj);
						schiebe(abi, abj, abi-1, abj, ca);
						checkABC = 1;
					}
			}

		break;	
	}
	
return checkABC;
}

//prüft ob es möglich ist mit einer Ecke zwei Mühlen zu öffnen (z.B. A7 und A1 besetzt, A6 und A2 frei, setzen auf A0)
//Setzt auf das entsprechende Feld
int pruefeEckeDoppelMuehle(int steinWert){
	printf("Sind in pruefeDM\n");
	int sw;
	sw = steinWert;
int tzi,tzj, tzjm1, tzjp1;
int tzreihe[4];
int tzcount = 0;
int pruefeEDM = 0;

for(tzi = 0; tzi < 3; tzi++){
	tzreihe[0] = 0;
	tzreihe[1] = 0;
	tzreihe[2] = 0;
	tzreihe[3] = 0;
	tzcount = 0;
	for(tzj = 1; tzj < 8; tzj +=2){
		tzjp1 = tzj+1;
		tzjm1 = tzj-1;
		if(tzj == 7){
			tzjp1 = 0;
		}
		printf("tzcount %i  tzi: %i tzj: %i %i %i\n", tzcount, tzi, tzj, tzjm1, tzjp1);
		if((shm_connectPiecelist->steine[tzi][tzj] == sw) && (shm_connectPiecelist->steine[tzi][tzjm1] + shm_connectPiecelist->steine[tzi][tzjp1]) == 0){
			tzreihe[tzcount] = 1;
			printf("IN IF: tzcount %i tzi: %i tzj: %i %i %i\n", tzcount, tzi, tzj, tzjm1, tzjp1);
		}	
		tzcount++;		
	}
	if((tzreihe[0] == 1) && (tzreihe[1] == 1)){
		setzte(tzi,2);
		pruefeEDM = 1;
		break;
	}
	else if ((tzreihe[1] == 1) && (tzreihe[2] == 1)){
		setzte(tzi, 4);
		pruefeEDM = 1;
		break;
	}
	else if ((tzreihe[2] == 1) && (tzreihe[3] == 1)){
		setzte(tzi,6);
		pruefeEDM = 1;
		break;
	}
	else if ((tzreihe[3] == 1) && (tzreihe[0] == 1)){
		setzte(tzi,0);
		pruefeEDM = 1;
		break;
	}
	printf("pruefeEDM: %i\n", pruefeEDM);
}
return pruefeEDM;
}

//Standard Funktion zum kompletten Prüfen des Umfelds eines Steins und entsprechendes reagieren
int checkUmfeld(int aci, int acj, int checkart, int ausi1, int ausj1, int ausi2, int ausj2){ //checkart == 0 -> freies Feld wird gesucht; checkart == 1 --> Stein wird auf übergebenes Feld geschoben
	int statusUmfeld = 0;
	int randswitch = 0;
	ca = checkart;

	printf("Wir sind in CheckUmfeld\n");

		if(acj % 2 == 0){
			statusUmfeld = checkCorner(aci, acj, ca, ausi1, ausj1, ausi2, ausj2);
			printf("umfeld %d\n",statusUmfeld );
		}

		else if(statusUmfeld == 0){
			randswitch = rand()%2;
			if(randswitch == 0){
				if((statusUmfeld = checkPM(aci, acj, ca, ausi1, ausj1, ausi2, ausj2)) == 0){			
				statusUmfeld = checkABC(aci, acj, ca, ausi1, ausj1, ausi2, ausj2);
				}
			}
			else{
				if((statusUmfeld = checkABC(aci, acj, ca, ausi1, ausj1, ausi2, ausj2)) == 0){			
				statusUmfeld = checkPM(aci, acj, ca, ausi1, ausj1, ausi2, ausj2);
				}
			}
		}
return statusUmfeld;
}

int openMill(int millsclosed){
	
int millsclsd = 0;
int statusOpenMill = 0;
millsclsd = millsclosed;
for(int i = 0; i < millsclsd; i++){
	
statusOpenMill = checkUmfeld(mmP1V[i], mmP2V[i],0,9,9,9,9);
		if(statusOpenMill == 0){
			statusOpenMill = checkUmfeld(millS1iV[i], millS1jV[i],0,9,9,9,9);
				if(statusOpenMill == 0){
					statusOpenMill = checkUmfeld(millS2iV[i], millS2jV[i],0,9,9,9,9);
				}
		}
	if(statusOpenMill == 1){break;}
}
return statusOpenMill;
}

//schlägt einen Stein der frei ist
/* TODO Suche wenn möglich zufällig bzw. sinnvoll [nicht aus Mühlen schlagen] */
void capture(){
	int done = 0;
	//char temp[2];
	buf[2] = '\0';
	buf[3] = '\0';
	buf[4] = '\0';

	for (int fi = 0; fi < 3; fi++)
		{
			for (int fj = 0; fj < 8; fj++)
			{
				if(shm_connectPiecelist->steine[fi][fj] == 5){
					setzte(fi, fj);
					shm_connectPiecelist->steine[fi][fj] = 0;
					done = 1;
					break;

				} 
			}

			if (done == 1) break;
		}
}

//allgemeine Funktion zur Regelung des Ablaufs (Capture, Setz-Phase oder Schiebephase)

char *logik(){

int doneL = 0;
ca = 0;
int ausi1, ausj1, ausi2, ausj2;
ausi1 = 9;
ausj1 = 9;
ausi2 = 9;
ausj2 = 9;

if(shm_connectPiecelist->anzahlGeschlageneSteine == 6){ //prüft ob wir bereits springen dürfen (6 geschlagene Steine)
springenErlaubt = 1;
}

srand(time(NULL));

	
if(shm_connectPiecelist->capture > 0){ //wir müssen einen Stein schlagen
	printf("Capture Stein");
	capture();
	doneL = 1;
}

else if(counterGesetzt<9){ //noch nicht alle Steine gesetzt

printf("Counter kleiner 9");
int eigenemuehle = 0;
//Prüft auf eigene und Fremdemühle und setzt entsprechend
eigenemuehle = checkMill(2);
printf("CheckMills\n");

	if(eigenemuehle == 1){
			setzte(mmP1, mmP2);
			counterGesetzt++;
			doneL = 1;
	}
		
	else if(eigenemuehle == 0){
		int fremdemuehle = 0;
		fremdemuehle = checkMill(10);
			if(fremdemuehle == 1){
				setzte(mmP1, mmP2);
				counterGesetzt++;
				doneL = 1;
			}
	}

	if(doneL != 1){//prüft auf mögliche DoppelMühle des Gegners
	printf("CheckDoppelMills\n");
		if(pruefeEckeDoppelMuehle(5) == 1){
			counterGesetzt++;
			doneL = 1;
		}
	else{
			// bzw. ob es möglich selbst eine solche Position zu erreichen
			printf("CheckEigeneDoppelMills\n");
			if(pruefeEckeDoppelMuehle(1) == 1){
				counterGesetzt++;
				doneL = 1;
			}
		}
	}


	int randC2 = 0;
	int randC5 = 0;

	int fertigcount = 0;

	int j, i;


	// Start mit B => qwi = 1
	int qwi = 1;
	int qwj = 0;
	//Zuerst die Mittelfelder testen
	int feldtyp = 1; //0 Ecke, 1 Mitte

	while((fertigcount < 6) && (doneL != 1)){
		
		j=0;
		int check[BUFSIZE];
		check[0] = 0;
		check[1] = 0;
		check[2] = 0;
		check[3] = 0;
		check[4] = 0;
		check[5] = 0;
		check[6] = 0;
		check[7] = 0;
		
		while(j<4){
				switch (feldtyp){
				case 0:
					i = (rand()%4) * 2;
					break;
				case 1:
					i = ((rand()%4) * 2)+1;
					break;
				}
				if (check[i] == 0){ 
					check[i] = 1;
					qwj = i;
					j++;
					
					if(shm_connectPiecelist->steine[qwi][qwj] == 0){
						//stein auf i,j
						//printf("I : %d, J: %d, STEINE: %d\n", i,j , shm_connectPiecelist->steine[i][j]);
						setzte(qwi, qwj);
						counterGesetzt++;
						printf("COUNTER %d\n", counterGesetzt);
						doneL = 1;
						break;
					}
				}
			if (doneL == 1) {break;}	
		}
		
		if (doneL == 1) {break;}
		fertigcount++;
		
		switch(fertigcount){
		case 1: 
			qwi = (rand()%2) * 2;
			printf("qwi im Case2 %i", qwi);
			if(qwi == 0){randC2 = 2;}
			break;
		case 2:
			qwi = randC2;
			break;
		case 3:
			qwi = 1;
			feldtyp = 0;
			break;
		case 4:
			qwi = (rand()%2) * 2;
			printf("qwi im Case4 %i", qwi);
			if(qwi == 0){randC5 = 2;}
			feldtyp = 0;
			break;
		case 5:
			qwi = randC5;
			feldtyp = 0;
		break;
		}
	}	
}

//prüft, ob bereits gesprungen wurde ==> es darf auch jetzt gleich wieder gesprungen werden
else if (springenErlaubt == 1)
{
	printf("Springen erlaubt");
	springe();	
}

else{ //Schiebe-Phase
	printf("Schiebe-Phase\n");
	doneL = 0;
	ca = 0;
	ausi1 = 9;
	ausj1 = 9;
	ausi2 = 9;
	ausj2 = 9;
	
//unsere Steine sind "1" IMMER - Gegner "5" - Alle sind bereits gesetzt - freie Felder sind "0"

//Prüfen auf Möglichkeit zum Schließen einer eigenen/fremden Mühle
//Dabei auch ein Ausschlusskriterium, sodass kein Stein bewegt werden soll, der bereits in einer Mühle ist

	int eigenemuehle = 0;
	eigenemuehle = checkMillAlle(2);
	if(eigenemuehle >= 1){
		printf("Eigene Mühle gefunden");
		for(int tryclose = 0; tryclose < eigenemuehle; tryclose++){
			ausi1 = millS1iA[tryclose];
			ausj1 = millS1jA[tryclose];
			ausi2 = millS2iA[tryclose];
			ausj2 = millS2jA[tryclose];
			doneL = checkUmfeld(mmP1A[tryclose], mmP2A[tryclose], 1, ausi1, ausj1, ausi2, ausj2);
			if (doneL == 1) {break;}
		}
	}
	
	
	//prüft ob wir eine geschlossene Mühle haben und öffnet diese
	if(doneL == 0){
	printf("CheckOpenMill\n");
	int millsclosed = 0;
	millsclosed = checkMillClosed(3);
	if(millsclosed >= 1){
		doneL = openMill(millsclosed);
	}
	}
	
	
	if(doneL == 0){
	int fremdemuehle = 0;
	fremdemuehle = checkMillAlle(10);
		if(fremdemuehle >= 1){
		printf("Fremde Mühle gefunden");
			for(int tryclose = 0; tryclose < fremdemuehle; tryclose++){
				ausi1 = millS1iA[tryclose];
				ausj1 = millS1jA[tryclose];
				ausi2 = millS2iA[tryclose];
				ausj2 = millS2jA[tryclose];
				doneL = checkUmfeld(mmP1A[tryclose], mmP2A[tryclose], 1, ausi1, ausj1, ausi2, ausj2);
				if (doneL == 1) {break;}
			}
		}
	}

	if(doneL == 0){
	printf("keine Mühle gefunden - Random Zug\n");
	int j1 = 0;	
	ca = 0;
	ausi1 = 9;
	ausj1 = 9;
	ausi2 = 9;
	ausj2 = 9;
	
	
	int i1, i2, yxi, yxj;
	int check1[BUFSIZE];
	int check2[BUFSIZE];
	check1[0] = 0;
	check1[1] = 0;
	check1[2] = 0;
	check1[3] = 0;
	check1[4] = 0;
	check1[5] = 0;
	check1[6] = 0;
	check1[7] = 0;
	
	while (j1<3){
		printf("while j1 < 3");
		i1 = rand()%3;
			if (check1[i1] == 0){ 
				check1[i1] = 1;
				yxi = i1;
				j1++;
							
					check2[0] = 0;
					check2[1] = 0;
					check2[2] = 0;
					check2[3] = 0;
					check2[4] = 0;
					check2[5] = 0;
					check2[6] = 0;
					check2[7] = 0;
					doneL = 0;
					
					int j2 = 0;
					while(j2<8){
						printf("while j2 < 8, j2: %i", j2);
						i2 = rand()%8;
						
							if (check2[i2] == 0){ 
								check2[i2] = 1;
								yxj = i2;
								j2++;
								if(shm_connectPiecelist->steine[yxi][yxj] == 1){
									printf("Wir checken das Umfeld von Stein %d %d\n", yxi, yxj);
									doneL = checkUmfeld(yxi, yxj, ca, ausi1, ausj1, ausi2, ausj2);
								} 
							}
						if (doneL == 1) {break;}	
					}
			}
		if (doneL == 1) {break;}
	}
	}
}


if(doneL != 1){
	springenErlaubt = 1;
	springe();
}

shm_connectPiecelist->neuerZug = 1; 

strcpy(koord, buf);
return koord;
}

//Erste Funktion im File - Verweist auf alle anderen Funktionen, sendet Zug raus
void think(){
	
	printf("Wilkommen im Thinker!\n");
	if ( shm_connectPiecelist->neuePiecelist == 1){
		shm_connectPiecelist->neuePiecelist = 0;

	
	strcpy(zug, logik());
	
	write(fd[1], zug, 20);

	}
	else {
		error("Fehler im Thinker, Singal nicht gesetzt");
		
	}
}