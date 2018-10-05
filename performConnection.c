#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#define EXTERN
#include "./globale.h"

#define GAMEKINDNAME NMMorris
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUFSIZE 1024
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0) //bzero als macro fuer memset

char zug2[BUFSIZE];

void sendenAnServer(int sockfd, char *messageC){
    /*
     * Methode um Nachrichten an den Server zu schicken
     * Syntax: sendenAnServer(sockfd, "Nachricht")
     * -Stefan
     */
    bzero(buf, BUFSIZE);
    strcpy(buf, messageC);
    int n = write(sockfd, buf, strlen(buf));
    if (n < 0) {
        error("Fehler beim Schreiben in Socket\n"); //bei Fehler Abbruch
    }
    printf("C: %s", buf );
}

char *lesenVonServer(int sockfd){
    /*
     * Methode um Nachrichten vom Server zu lesen
     * Syntax: lesenVonServer(sockfd)
     * @return Nachricht vom Server als char
     * -Stefan
     */
    bzero(buf, BUFSIZE);
    int n = read(sockfd, buf, BUFSIZE);
    if (n < 0) {
        error("Fehler beim Lesen des Sockets\n"); //bei Fehler Abbruch
    }
    printf("LOG: %s", buf);
    strcpy(messageS, buf);
    return messageS;
}

void reagieren(int sockfd){
	/*Client im Wartezustand - wartet auf Nachricht vom Server */
	printf("Reagieren\n");
	
	lesenVonServer(sockfd);
	
	if(strncmp(messageS, "+ WAIT", 6) == 0){
		//sendet OKWAIT falls WAIT Signal vom Server kommt
		sendenAnServer(sockfd, "OKWAIT\n");
		//auf weitere Nachrichten im gleichen String prüfen
		if(strstr(messageS, "+ MOVE")){
			printf("++++++++++++++++++++++++++++++\n + MOVE im String mit + WAIT\n +++++++++++++\n");
			printf("Neuer Zug vom Server - nach WAIT im gleichen String\n");
			sendenAnServer(sockfd, "THINKING\n");
			serverStringTeilen(messageS);
		}
		else if(strstr(messageS,"+ GAMEOVER")){
			printf("Spiel vorbei(2)\n");
			sleep(2);
			exit(0);
		}
	}
	else if(strncmp(messageS, "+ OKTHINK", 9) == 0){
			printf("Zug ausführen\n");
			
			// Es gibt eine neue Piecelist
			shm_connectPiecelist->neuePiecelist = 1; 
			
			// Signal an thinker zum starten des denkprozesses
			kill(shm_connect->vaterPid, SIGUSR1);
			
			// Warten auf Thinker - hier ggf. dynamische Wartezeit
			while(shm_connectPiecelist->neuerZug == 0){
				//printf("Warten auf Berechnung\n");
			} 
			
			//Lesen aus Pipe
			read(fd[0], &zug2, 20);
					
			printf("Zug: %s\n", zug2);
			
			//Richtigen String zusammensetzen
			char playstring[20] = "PLAY ";
			strcat(playstring, zug2);
			strcat(playstring, enter);
			
			//String an Server senden
			sendenAnServer(sockfd, playstring); 
			
			shm_connectPiecelist->neuerZug = 0; //Zug Benachrichtung wieder null setzen
	}
	else if(strncmp(messageS, "+ MOVEOK", 8) == 0){
		printf("Server denkt\n");
	}
	else if(strncmp(messageS, "+ MOVE", 6) == 0){
		printf("Neuer Zug vom Server\n");
		sendenAnServer(sockfd, "THINKING\n");
		/*Test - Funktion zum prüfen der korrektem Implementierung Erkennung mehrerer Befehle
		if(strstr(messageS, "+ CAPTURE")){
		
		printf("++++++++++++++++++++\n ERKENNT die Zweite Stelle nach + MOVE korrekt!\n +++++++++++++++++++++++++++++\n");
		
		}
		*/
		serverStringTeilen(messageS);
	}
	else if(strncmp(messageS, "+ GAMEOVER", 10) == 0){
		printf("Spiel vorbei\n");
		sleep(2);
		exit(0);
		//evtl. schöne Ausgabe wer gewonnen hat
	}
	else if(strncmp(messageS, "-", 1) == 0){
			error("Fehler - '-' vom Server\n");
	}

	if(!*messageS){ //prüft auf Leer-String von messageS und beendet dann - falls der Ende-String des Servers nicht erkannt wurde
			printf("String leer");
			sleep(5);
			exit(0);
	}
	
	reagieren(sockfd);
}

void performConnection(int sockfd){
    /*
     * Kommunikation mit dem Server
     * Exessive Fehlerbehandlung notwendig
     * -Stefan
     */

	if(strncmp(lesenVonServer(sockfd), "+", 1) == 0){ //Server bestätigt die Verbindung
		sendenAnServer(sockfd, "VERSION 2.3\n");
	}
	else{
		error("Fehler");
	}
	if(strncmp(lesenVonServer(sockfd), "+", 1) == 0){ //Server akzeptiert und möchte Game-ID
    	sendenAnServer(sockfd, id);
	}
	else{
		error("Fehler");
	}
	if(strncmp(lesenVonServer(sockfd), "+", 1) == 0){ //Server sendet Spielart und Datum
		char spielname[BUFSIZE];
		char datumspiel[BUFSIZE];
		strcpy(spielname, split(messageS, 3));
		strcpy(datumspiel, split(messageS, 7));
		printf("Wir spielen %s. Das Datum des Spiels ist: %s\n", spielname, datumspiel);
    	sendenAnServer(sockfd, player); //sendet Player ID - meist leer
    	//Für SHM Struktur gameInfo, gamename:
    	strcpy(shm_connect->gamename, spielname);
	}
	else{
		error("Fehler");
	}
  	
	/* Prolog abgeschlossen */
	
	
	if(strncmp(lesenVonServer(sockfd), "+", 1) == 0){ //sendet Spieler Nummern und Anzahl der Spieler

	//Für SHM Struktur gameInfo, Spielernummer:
		shm_connect->unsereSpielerNummer=(int)strtol(split(messageS, 3), NULL, 10);
	//Für SHM Struktur gameInfo, Spieleranzahl:
        shm_connect->spielerAnzahl=(int) strtol(split(messageS, 8),NULL, 10);
        printf("Spielernummer: %d\n", shm_connect->unsereSpielerNummer);
        printf("spielerAnzahl: %d\n", shm_connect->spielerAnzahl);
	
		//Warten auf MOVE Signal des Servers
		if(strncmp(split(messageS, 17), "MOVE", 4) == 0 ){
		
			sendenAnServer(sockfd, "THINKING\n");
			serverStringTeilen(messageS);
			
			//Server Signal verarbeiten
			reagieren(sockfd);
		}
	
		//Wenn nicht, dann Wartemodus
		else {
			reagieren(sockfd);
		}
	}
}