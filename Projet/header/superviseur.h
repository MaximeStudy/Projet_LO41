#ifndef _superviseurH_
#define _superviseurH_
#include <sys/time.h>

/* Convoyeur*/
int tailleConv;
piece * conv; //convoyeur
piece* pieceVideConv; //reference piece vide du convoyeur
pthread_t thread_superviseur;

pthread_mutex_t MitSurRobotAlim;
pthread_mutex_t AttentRetrait;
pthread_mutex_t mutexAlim;
pthread_mutex_t mutexRetrait;
pthread_cond_t RobotAlim;
pthread_cond_t RobotRetrait;
pthread_mutex_t mutexSuiviAlim;
pthread_mutex_t mutexSuiviRetrait;
pthread_cond_t RobotSuiviAlim;
pthread_cond_t RobotSuiviRetrait;

pthread_t thread_SuiviRobotAlim;
pthread_t thread_SuiviRobotRetrait;

llist ajouterEnFin(llist liste, piece valeur);
llist supprimerElementEnTete(llist liste);
piece * recupererElementEnTete(llist liste);
void afficherListe(llist liste);
llist effacerListe(llist liste);
void creerPiece(int ope);
void * threadSuiviMachine(void * arg);
void initaliserSuiviMachine();
void Superviseur();
void * fonc_SuiviRobotAlim(void * arg);
void * fonc_SuiviRobotRetrait(void * arg);

piece *pieceRobotAlim;
int EnMarche;
#endif
