#include "../header/convoyeur.h"

void initialiserConvoyeur(){
    num=0; //compteur pour les id pieces
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
