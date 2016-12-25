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
  initaliserSuiviMachine();

  sleep(2);

  int i;
  for(i=0;i<24;i++)
  {
       creerPiece(i%4);
  }
   /* Creation convoyeur */

  for(i=0;i<NbMachine;i++)
  {
    //printf("\nList %d\n",i);
    //afficherListe(maListeMachine[i]->listeAttente);
    //effacerListe(maListeMachine[i]->listeAttente); // Libère les ressources
  }

  //piece *p=recupererElementEnTete(maListeMachine[0]->listeAttente);

  //printf("recuperation de %d\n", p->num);
  initialiserConvoyeur();

  //ajouterPieceConvoyeur(0,*p);
  afficherConvoyeur();


  sleep(600);
  printf("nb attente : %d",nbAttente);
  for(i=0;i<NbMachine;i++)
  {
    printf("\nList %d\n",i);
    afficherListe(maListeMachine[i]->listeAttente);
    //effacerListe(maListeMachine[i]->listeAttente); // Libère les ressources
  }

  libererConvoyeur();

  return EXIT_SUCCESS;
}
