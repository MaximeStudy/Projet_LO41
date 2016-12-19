/* Convoyeur*/
int tailleConv;
piece * conv; //convoyeur
piece* pieceVideConv; //reference piece vide du convoyeur

void initialiserConvoyeur();
void libererConvoyeur();
void tournerConvoyeur();
void ajouterPieceConvoyeur(int position, piece p);
void afficherConvoyeur();
llist ajouterEnFin(llist liste, piece valeur);
llist supprimerElementEnTete(llist liste);
piece * recupererElementEnTete(llist liste);
void afficherListe(llist liste);
llist effacerListe(llist liste);
void creerPiece(int ope);
