#ifndef _superviseurH_
#define _superviseurH_
#include <sys/time.h>

/* Convoyeur*/
int tailleConv;
piece * conv; //convoyeur
piece* pieceVideConv; //reference piece vide du convoyeur


llist ajouterEnFin(llist liste, piece valeur);
llist supprimerElementEnTete(llist liste);
piece * recupererElementEnTete(llist liste);
void afficherListe(llist liste);
llist effacerListe(llist liste);
void creerPiece(int ope);
void * threadSuiviMachine(void * arg);
void initaliserSuiviMachine();

#endif
