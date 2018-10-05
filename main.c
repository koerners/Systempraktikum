#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h> //für warten auf Kindprozess
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

// fuer Shared Memory:
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define GAMEKINDNAME NMMorris
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUFSIZE 1024
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0) //bzero als macro fuer memset

#define EXTERN
#include "./globale.h"

EXTERN char id[19]= "ID ";
EXTERN char enter[3] = "\n";
EXTERN char player[BUFSIZE] = "PLAYER";
#define SHM_GAMESIZE sizeof(gameInfo)
#define SHM_PLAYERSIZE sizeof(playerInfo)
#define SHM_PIECESIZE sizeof(piecelist)

int shm_gameid; //IDs von SHM
int shm_playerid;
int shm_piecelistid;
gameInfo *shm_connect; // Für Fehlerbehandlung bei shmat()
playerInfo *shm_connectPlayer;
piecelist *shm_connectPiecelist;


void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        //printf("Received SIGUSR1!\n");
        think();
    }
}


void error(char *msg) {
/* Wrapper für perror
 * gibt bessere Fehlermeldungen
 * idealerweise...
 */
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
	
	/* Kommandoparameter einlesen
     * Hier werden die Usereingaben gameID und Spielernummer aufgenommen und in die globalen Variablen gespeichert
     * Muss wohl so in der Main Funktion stehen bleiben
     * Evtl. noch checken ob das Format stimmt, also |GameID| == 16 und Spielernummer = 1 || 2
     * - Stefan
     */
   
    for(int i = 0; i<argc; i++ ){
        if(strcmp(argv[i], "-g") == 0){
            strcpy(gameID, argv[i+1]);
        }
        if(strcmp(argv[i], "-p") == 0){
            strcpy(gewuenschteSpielernummer, argv[i+1]);
        }
		if(strcmp(argv[i], "-cf") == 0){ //Liest den Dateinamen der Config.conf ein
            strcpy(configfile, argv[i+1]); /* Prüfung auf Existenz der Datei erst bei Verarbeitung */
        }
    }
	
	oeffneConfigDatei(configfile);
	printf("Port %i\n", globalconf.portnummer);
	printf("Host %s\n", globalconf.hostname);
	printf("Spiel %s\n", globalconf.spielart);
	
	//strcpy(gameID,"2csv4nhlmwgbw"); //setzt Game-ID Statisch fest
    strcat(id, gameID); //fügt gameID an "ID " an
    strcat(id, enter); //ergänzt enter nach "ID GameID"
   	
   	/* Hier die Anenderungen für funktionierendes Skript */
    int number1 = atoi(gewuenschteSpielernummer);
	char string1[BUFSIZE];
	if(number1 != 0){
		number1--;
		sprintf(string1, " %d",number1);
		strcat(player, string1);
	}
    /*Im Zweifel wieder löschen und string1 wieder in gewuenschteSPielernummer ändern */

    strcat(player, enter);

    /* ENDE Kommandoparameter einlesen */

    /*
     * Verbindung zum Server herstellen
     *  -Stefan
     **/

    int sockfd, portno;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
	
	hostname = globalconf.hostname;
	portno = globalconf.portnummer;

    /* Socket wird erstellt */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Fehler beim Erstellen des Sockets");

    /* DNS Eintrag */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"Host nicht vorhanden %s\n", hostname);
        exit(0);
    }

    /* Internetadresse des Servers erstellen */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* Verbinden mit Server */
    if (connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
        error("Fehler beim Verbinden");
	}
    /* ENDE Verbindung zum Server herstellen */
    
    
    /*Shared Memory segment erstellen: (Was ist das Problem?)
     Größe des Speichers erst zu compilezeit bekannt --> mit malloc()?
     Beide Strukturen in ein Segment? --> Größenproblem wegen sizeof(gameInfo)
     */
    shm_gameid = shmget(IPC_PRIVATE, SHM_GAMESIZE, IPC_CREAT | SHM_R | SHM_W);
    if(shm_gameid==-1) {
        error("Bei der Erstellung des Shared Memory Segments-game ist etwas schiefgelaufen");
    }
    shm_playerid = shmget(IPC_PRIVATE, SHM_PLAYERSIZE, IPC_CREAT | SHM_R | SHM_W);
    if(shm_playerid==-1) {
        error("Bei der Erstellung des Shared Memory Segments-player ist etwas schiefgelaufen");
    }
     shm_piecelistid = shmget(IPC_PRIVATE, SHM_PIECESIZE, IPC_CREAT | SHM_R | SHM_W);
    if(shm_piecelistid==-1) {
        error("Bei der Erstellung des Shared Memory Segments-piecelist ist etwas schiefgelaufen");
    }
	
    /*Prozess wird mit SHM verbunden, somit später Vater und Kind
     Frage: -mit beiden Segmenten verbinden oder beide Segmente zu einem machen (WIE)
            -wie bekomme ich die Strukturen ins SHM?
     */
    shm_connect = shmat(shm_gameid, NULL, 0);
    if (shm_connect == (void *) -1) {
        error("Verbindung zu SHM-gameInfo fehlgeschlagen");
    }
    shm_connectPlayer = shmat(shm_playerid, NULL, 0);
    if(shm_connectPlayer==(void*)-1){
    	error("Verbindung zu SHM-playerInfo fehlgeschlagen");
    }
     shm_connectPiecelist = shmat(shm_piecelistid, NULL, 0);
    if(shm_connectPiecelist==(void*)-1){
    	error("Verbindung zu SHM-piecelist fehlgeschlagen");
    }
    
	/* Pipe wird erstellt */

	pipe(fd); //fd global bekannt und deklariert
	
	/*KindProzess wird erstellt - Connector ist der Kindprozess */
		
	pid_t pid;
	switch (pid = fork()){
	case -1:
		//Fehler
		error("Fehler bei Erstellung des Kindprozesses");
		break;
		
	case 0:
		/* Kindprozess = Connector */
		close(fd[1]);
		performConnection(sockfd);
		break;
       
	default:
		/* Elternprozess = Thinker */
          /*Belegung der Struktur gameInfo:
             Spieleranzahl auslesen --> array vom typ playerInfo mit enstprechend großem speicher und malloc
          */
		close(fd[0]);
        shm_connect->kindPid = pid;
        shm_connect->vaterPid = getpid();
       
        signal(SIGUSR1, my_handler); //Reagieren auf Signal des Kindprozesses - Startet Thinker
//Sleep hier notwendig??
       sleep(1);
        
		wait(NULL); //Warten auf Kindprozess
		printf("Kind beendet\n");
		
		if(shmdt(shm_connect)==-1){
			error("Detach fehlgeschlagen");
		}
		else{
			printf("Detach erfolgreich\n");
		}
		
		close(sockfd); //-> Am Schluss nicht vergessen
		break;  
	}
	
return 0;
}