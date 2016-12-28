#include "../header/IHM.h"
#define PAR_DEFAUT 1
#define PERSONALISE 2
#define DEFAILLANCE 3
#define QUIT 0


int selectionChoix(void)
{
  int option;
  printf("Menu de selection :\n\n");
  printf("1.\tPar defaut\n");
  printf("2.\tMode personalisé\n");
  printf("3.\tMode défaillance\n");
  printf("0.\tQuit\n\n");
  printf("Faite votre choix : ");
  while( (scanf(" %d", &option) != 1) || (option < 0)  || (option > 3))  // probleme si on rentre un char !!!!
  {
    fflush(stdin);                    /* clear bad data from buffer */
    printf("La selection n'est pas valide. Essayez à nouveau.\n\n");
    printf("Votre choix ? ");
  }
    return option;
}

void faireQuitter()
{
    printf("Au revoir !\n");
    exit(0);
}
void faireParDefaut()
{
  printf("Menu defaut : \n");
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
    effacerListe(maListeMachine[i]->listeAttente); // Libère les ressources
 }

  libererConvoyeur();
}

void menu(void) {
    int choix;     // main variables

    choix = selectionChoix();   // get user's first selection

    while(1)   //execute so long as choice is not equal to QUIT
    {
        switch(choix)
            {
                case PAR_DEFAUT :
                    faireParDefaut();
                    break;
                case PERSONALISE :
                    printf("Menu personalise : \n");
                    break;
                case DEFAILLANCE:
                    printf("Menu défailance : \n");
                    break;
                case QUIT:
                    faireQuitter();
                    break;
                default:    printf("Oups! Une erreur dans le choix du menu est survenu. ");
                            printf("Veuillez réessayer svp.\n");
            }
      choix = selectionChoix(); /* recommence la selection */
   }
 }

void lancerIHM(void) {
  menu();
  // creationMachines();
  // creationRobots();
  // //initaliserSuiviMachine();
  // Superviseur();
  //
  // sleep(2);
  //
  // int i;
  // for(i=0;i<24;i++)
  // {
  //      creerPiece(i%4);
  // }
  //
  //  /* Creation convoyeur */
  //
  // initialiserConvoyeur();
  //
  // afficherConvoyeur();
  //
  //
  // sleep(600);
  // printf("nb attente : %d",nbAttente);
  // for(i=0;i<NbMachine;i++)
  // {
  //   printf("\nList %d\n",i);
  //   afficherListe(maListeMachine[i]->listeAttente);
  //   //effacerListe(maListeMachine[i]->listeAttente); // Libère les ressources
//  }

  //libererConvoyeur();
}
