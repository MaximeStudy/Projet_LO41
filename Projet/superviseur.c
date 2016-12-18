#include <stdlib.h>
#include <stdio.h>

struct piece {
  int num;
  int ope; //operation d'usinage
  int estUsine; //pour svaoir si la piece est usine (necessaire ?)
};
typedef struct piece piece;
int num=0; //compteur
int nbOpe=4;

/* Liste chainée*/
struct element
{
    piece val;
    struct element *nxt;
};
typedef struct element element;
typedef element* llist;
llist * mesListeChaineOp;

llist ajouterEnFin(llist liste, piece valeur)
{
    /* On crée un nouvel élément */
    element* nouvelElement = malloc(sizeof(element));

    /* On assigne la valeur au nouvel élément */
    nouvelElement->val = valeur;

    /* On ajoute en fin, donc aucun élément ne va suivre */
    nouvelElement->nxt = NULL;

    if(liste == NULL)
    {
        /* Si la liste est videé il suffit de renvoyer l'élément créé */
        return nouvelElement;
    }
    else
    {
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
        element* temp=liste;
        while(temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = nouvelElement;
        return liste;
    }
}

llist supprimerElementEnTete(llist liste)
{
    if(liste != NULL)
    {
        /* Si la liste est non vide, on se prépare à renvoyer l'adresse de
        l'élément en 2ème position */
        element* aRenvoyer = liste->nxt;
        /* On libère le premier élément */
        free(liste);
        /* On retourne le nouveau début de la liste */
        return aRenvoyer;
    }
    else
    {
        return NULL;
    }
}

void afficherListe(llist liste)
{
    element *tmp = liste;
    /* Tant que l'on n'est pas au bout de la liste */
    printf("*****************************************\n");
    while(tmp != NULL)
    {
        /* On affiche */
        printf("num   : %5d ", tmp->val.num);
        printf("Ope   : %5d ", tmp->val.ope);
        printf("Usine : %5d\n", tmp->val.estUsine);
        /* On avance d'une case */
        tmp = tmp->nxt;
    }
}

llist effacerListe(llist liste)
{
    element* tmp = liste;
    element* tmpnxt;

    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)
    {
        /* On stocke l'élément suivant pour pouvoir ensuite avancer */
        tmpnxt = tmp->nxt;
        /* On efface l'élément courant */
        free(tmp);
        /* On avance d'une case */
        tmp = tmpnxt;
    }
    /* La liste est vide : on retourne NULL */
    return NULL;
}

void creerPiece(int ope)
{
  piece* nouvellePiece = malloc(sizeof(piece));
  nouvellePiece->num = num;
  nouvellePiece->ope = ope;
  nouvellePiece->estUsine =0;
  num++;
  mesListeChaineOp[nouvellePiece->ope]=ajouterEnFin(mesListeChaineOp[nouvellePiece->ope], *nouvellePiece);
}

int main(int argc, char **argv)
{
  int i;
  mesListeChaineOp=malloc(nbOpe*sizeof(llist));

  for(i=0;i<nbOpe;i++)
  {
    llist nouvelleList = NULL; // null obligatoire sinon considere comme contenant un element
    mesListeChaineOp[nbOpe]=nouvelleList;
  }
  for(i=0;i<100;i++)
  {
      creerPiece(i%4);
  }
  for(i=0;i<nbOpe;i++)
  {
    printf("\nList %d\n",i);
    afficherListe(mesListeChaineOp[i]);
    effacerListe(mesListeChaineOp[i]); // Libère les ressources
  }


  return 0;
}
