#ifndef _superviseurH_
#define _superviseurH_

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

#endif
