#ifndef _convoyeurH_
#define _convoyeurH_

#include "../header/machine.h"
#include "../header/superviseur.h"
pthread_t thread_convoyeur;
pthread_mutex_t mutexConvoyeur;
int vitesseConv;
int affichage;
void initialiserConvoyeur(int vit);
void libererConvoyeur();
void tournerConvoyeur();
void ajouterPieceConvoyeur(int position, piece p);
piece retirerPieceConvoyeur(int position);
void afficherConvoyeur();

#endif
