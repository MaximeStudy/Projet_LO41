#include <stdlib.h>
#include <stdio.h>
#include "../header/superviseur.h"

void initialiserConvoyeur(){
    tailleConv=10;
    int i;
    conv=malloc(tailleConv*sizeof(piece));
    pieceVideConv = malloc(sizeof(piece));
    pieceVideConv->num = -1;
    pieceVideConv->ope = -1;
    pieceVideConv->estUsine = -1;

    for (i =0 ; i<tailleConv; i++){
        conv[i] = *pieceVideConv;
    }
}

void libererConvoyeur(){
    free(pieceVideConv);
    free(conv); //libère les ressources alloué
}

void tournerConvoyeur()
{
  piece tmp=conv[tailleConv-1];
  int i;
  for(i=tailleConv-1;i>0;i--)
  {
    conv[i]=conv[i-1];
  }
  conv[0]=tmp;
}

void ajouterPieceConvoyeur(int position, piece p)
{
  if(position<tailleConv && position>=0)
  {
    conv[position]=p;
  }
}

void afficherConvoyeur()
{
    /* Tant que l'on n'est pas au bout de la liste */
    printf("*****************************************\n");
    int i;
    for(i=0;i<tailleConv;i++)
    {
        /* On affiche */
        if(conv[i].num!=-1)
        {
          printf("conv[%2d] : %5d \n",i, conv[i].num);
        }
        else {
          printf("conv[%2d] : %5s \n",i,"vide");
        }
    }
}

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

piece * recupererElementEnTete(llist liste)
{
    if(liste != NULL)
    {
        /* Si la liste est non vide, on se prépare à renvoyer l'adresse de
        l'élément en 2ème position */
        piece *aRenvoyer = &(liste->val);
        /* On retourne le nouveau début de la liste */
        return aRenvoyer;
    }
    else
    {
        return NULL; //si aucun element dans la liste
    }
}

void afficherListe(llist liste)
{
    printf("*****************************************\n");
    if(liste==NULL)
    {
      printf("la liste est vide\n");
    }
    else {
      element *tmp = liste;

      /* Tant que l'on n'est pas au bout de la liste */
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

void initaliserListeChaineOp()
{
  num=0; //compteur pour les id pieces
  nbOpe=4; //nb d'operation differente
  int i;
  mesListeChaineOp=malloc(nbOpe*sizeof(llist));

  for(i=0;i<nbOpe;i++)
  {
    llist nouvelleList = NULL; // null obligatoire sinon considere comme contenant un element
    mesListeChaineOp[nbOpe]=nouvelleList;
  }
}
/* Suivi machine du superviseur */
/*
void initaliserSuiviMachine()
{
  for (i = 0; i < NbMachine; i++) {
    machine * nouvelleMachine = malloc(sizeof(machine));
    nouvelleMachine->numMachine=i;
    nouvelleMachine->sleepTime=i+2;
    pthread_create(&(nouvelleMachine->thread_id), &thread_attr, fonc_machine, nouvelleMachine);
    printf("Main: thread numero %d creee: id = %ld\n",nouvelleMachine->numMachine,(nouvelleMachine->thread_id));
  }
} */
/*
void * threadMachine(void * arg) {
  machine * ma=(machine *)arg;
  while(1) {

  }
  pthread_exit(NULL);
} */
