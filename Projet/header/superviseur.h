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

llist ajouterEnFin(llist liste, piece valeur);
llist supprimerElementEnTete(llist liste);
piece * recupererElementEnTete(llist liste);
void afficherListe(llist liste);
llist effacerListe(llist liste);
void creerPiece(int ope);
void * threadSuiviMachine(void * arg);
void initaliserSuiviMachine();
void initaliserSuperviseur();
void * threadSuperviseur(void * arg);

piece *pieceRobotAlim;
#endif
