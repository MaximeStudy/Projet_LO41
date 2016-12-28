#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../header/machine.h"
#include "../header/robot.h"
#include "../header/superviseur.h"
#include "../header/convoyeur.h"
#include "../header/IHM.h"
int main()
{
  creationMachines();
  creationRobots();
//initaliserSuiviMachine();
Superviseur();

  sleep(2);

  int i;
  for(i=0;i<24;i++)
  {
       creerPiece(i%4);
  }

   /* Creation convoyeur */

  initialiserConvoyeur();

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
