#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../header/machine.h"
#include "../header/robot.h"
#include "../header/superviseur.h"
#include "../header/convoyeur.h"

int main()
{
  creationMachines();
  creationRobots();
  int i;
  for(i=0;i<100;i++)
  {
       creerPiece(i%4);
  }
   /* Creation convoyeur */

  for(i=0;i<NbMachine;i++)
  {
    printf("\nList %d\n",i);
    afficherListe(maListeMachine[i]->listeAttente);
    //effacerListe(maListeMachine[i]->listeAttente); // Libère les ressources
  }

  piece *p=recupererElementEnTete(maListeMachine[0]->listeAttente);

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
