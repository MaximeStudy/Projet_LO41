#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../header/superviseur.h"
#include "../header/machine.h"
#include "../header/IHM.h"

int main()
{
  creationMachines(); //erreur
  initaliserListeChaineOp();
  int i;
  for(i=0;i<100;i++)
  {
       creerPiece(i%4);
  }
   /* Creation convoyeur */

  for(i=0;i<nbOpe;i++)
  {
    printf("\nList %d\n",i);
    afficherListe(mesListeChaineOp[i]);
    effacerListe(mesListeChaineOp[i]); // Libère les ressources
  }

  piece *p=recupererElementEnTete(mesListeChaineOp[0]);

  //printf("recuperation de %d\n", p->num);
  initialiserConvoyeur();

  ajouterPieceConvoyeur(0,*p);
  afficherConvoyeur();


  for(int k=0;k<20;k++)
  {
    sleep(1);
    tournerConvoyeur();
    afficherConvoyeur();
  }

  libererConvoyeur();

  return EXIT_SUCCESS;
}
