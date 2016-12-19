#ifndef _convoyeurH_
#define _convoyeurH_

#include "../header/machine.h"
#include "../header/superviseur.h"

void initialiserConvoyeur();
void libererConvoyeur();
void tournerConvoyeur();
void ajouterPieceConvoyeur(int position, piece p);
void afficherConvoyeur();

#endif
