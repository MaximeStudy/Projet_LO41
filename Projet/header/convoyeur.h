#ifndef _convoyeurH_
#define _convoyeurH_

#include "../header/machine.h"
#include "../header/superviseur.h"
pthread_t thread_convoyeur;
//pthread_mutex_t mutexConvoyeur;
int vitesseConv;
void initialiserConvoyeur();
void libererConvoyeur();
void tournerConvoyeur();
void ajouterPieceConvoyeur(int position, piece p);
void afficherConvoyeur();

#endif
