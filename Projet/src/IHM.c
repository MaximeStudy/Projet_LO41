#include "../header/IHM.h"
#define PAR_DEFAUT 1
#define PERSONNALISE 2
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
  int nombreMachine=4;
  int vitesseC=1;
  printf("Menu defaut : \n\n");


  creationMachines(nombreMachine); //temps usinage de machines est i+1
  creationRobots();
  initialiserConvoyeur(vitesseC);
  Superviseur();

  sleep(1); //attendre que les threads soient bien en place

  int i;
  for(i=0;i<8;i++)
  {
       creerPiece(i%4);
  }

   /* Creation convoyeur */

  //afficherConvoyeur();
  sleep(34);
  printf("nb attente : %d",nbAttente);
  for(i=0;i<NbMachine;i++)
  {
    printf("\nList %d\n",i);
    afficherListe(maListeMachine[i]->listeAttente);
    //effacerListe(maListeMachine[i]->listeAttente); // Libère les ressources
 }
  //libererConvoyeur();
  exit(0);
}

void fairePerso()
{
  int nombreMachine;
  int vitesseC;
  printf("Menu personnalisé : \n");
  printf("Nombre de machine total ?\n");
  scanf(" %d", &nombreMachine);
  printf("Vitesse du convoyeur ?\n");
  scanf("%d", &vitesseC);
  int i;
  int j;
  int tab[nombreMachine];
  for(i=0;i<nombreMachine;i++)
  {
    printf("Nombre de pieces machine %d ?\n",i);
    scanf("%d",tab+i); //TODO verif int dans le scan f
  }
  creationMachines(nombreMachine); //temps usinage de machines est i+1
  creationRobots();
  initialiserConvoyeur(vitesseC);
  Superviseur();

  sleep(2); //attendre que les threads soient bien en place

  for(i=0;i<nombreMachine;i++)
  {
    for(j=0;j<tab[i];j++)
    {
      creerPiece(j);
    }
  }
  affichage=1;


   /* Creation convoyeur */

  //afficherConvoyeur();
  sleep(34);
  affichage=0;

  printf("nb attente : %d",nbAttente);
  for(i=0;i<NbMachine;i++)
  {
    printf("\nList %d\n",i);
    afficherListe(maListeMachine[i]->listeAttente);
    //effacerListe(maListeMachine[i]->listeAttente); // Libère les ressources
 }
  //libererConvoyeur();
  exit(0);
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
                case PERSONNALISE :
                    fairePerso();
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
