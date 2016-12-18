/* piece */
struct piece {
  int num;
  int ope; //operation d'usinage
  int estUsine; //pour svaoir si la piece est usine (necessaire ?)
};
typedef struct piece piece;
int num; //compteur
int nbOpe; //nb d'operation differente

/* Liste chainée*/
struct element
{
    piece val;
    struct element *nxt;
};
typedef struct element element;
typedef element* llist;
llist * mesListeChaineOp;

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
void initaliserListeChaineOp();
