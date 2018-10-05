#ifndef EXTERN
#define EXTERN extern
#endif

#define BUFSIZE 1024

EXTERN char gameID[13];
EXTERN char configfile[BUFSIZE];
EXTERN char gewuenschteSpielernummer[BUFSIZE];

EXTERN char id[19];
EXTERN char enter[3];
EXTERN char player[BUFSIZE];

EXTERN char buf[BUFSIZE];
EXTERN char messageC[BUFSIZE];
EXTERN char messageS[BUFSIZE];
EXTERN char newString[256][256]; 
EXTERN char stringGeteilt[BUFSIZE][BUFSIZE];
EXTERN int fd[2];

void warten(int sockfd);
void reagieren(int sockfd);
void speicherPieceStart(char *string, int piecenr);
void error(char *msg);

void sendenAnServer(int sockfd, char *messageC);
char *lesenVonServer(int sockfd);
void performConnection(int sockfd);
char *splitString(char *str, int positionWort);
char *split(char *string, int position);
void serverStringTeilen(char *string);

//structure die sich in shm befinden soll:
typedef struct{
    char gamename[BUFSIZE];
    int unsereSpielerNummer;
    int spielerAnzahl;
    pid_t vaterPid;
    pid_t kindPid;
} gameInfo;

typedef struct{
    int spielerNummer;
    char name[BUFSIZE];
    int status; //0 für false, 1 für true -> ready y/n
} playerInfo;

typedef struct{
	int steine[3][8];
	int capture;
	int neuePiecelist;
	int	anzahlGeschlageneSteine;
	int neuerZug;
} piecelist;

gameInfo *shm_connect; // Für Fehlerbehandlung bei shmat()
playerInfo *shm_connectPlayer; 
piecelist *shm_connectPiecelist;


typedef struct{
    char hostname[BUFSIZE];
    int portnummer;
    char spielart[BUFSIZE];
}config;

EXTERN config globalconf;

void leseConfig(); //um eine Default Datei einzulesen
void oeffneConfigDatei(char *dateiname); //um den übergebenen Dateinamen einzulesen
void think();