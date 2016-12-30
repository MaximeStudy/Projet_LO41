#include "../header/IHM.h"
#define PAR_DEFAUT 1
#define PERSONNALISE 2
#define DEFAILLANCE 3
#define QUIT 0

int selectionChoixDefaillance(void)
{
  int option;
  printf("\nMenu de sélection de défaillances :\n\n");
  printf("\t1.\tRobot alimentation : Placement sur convoyeur\n");
  printf("\t2.\tMachine : Retirer piece du convoyeur\n");
  printf("\t3.\tMachine : travail long \n");
  printf("\t0.\tQuit\n\n");
  printf("Faite votre choix : ");
  while( (scanf(" %d", &option) != 1) || (option < 0)  || (option > 3))  // probleme si on rentre un char !!!!
  {
    fflush(stdin);                    /* clear bad data from buffer */
    printf("La selection n'est pas valide. Essayez à nouveau.\n\n");
    printf("Votre choix ? ");
  }
    return option;
}

int selectionChoix(void)
{
  char option[1];
  int res;
  printf("\nMenu de sélection :\n\n");
  printf("1.\tPar défaut\n");
  printf("2.\tMode personnalisé\n");
  printf("3.\tMode défaillance\n");
  printf("0.\tQuit\n\n");
  printf("Faite votre choix : ");
  scanf("%s", option);
  res = atoi(option);
  printf("%d\n",res);
  while( (res != 1) || (res < 0)  || (res > 3)) 
  {
    fflush(stdin);                    /* clear bad data from buffer */
    printf("La selection n'est pas valide. Essayez à nouveau.\n\n");
    printf("Votre choix ? ");
    scanf(" %s", option);
  res = atoi(option);
  printf("%d\n",res);
  }
    return res;
}

void faireQuitter()
{
    printf("Au revoir !\n");
    exit(0);
}

void faireParDefaut(void) //debug 0 pour lancer l'anomalie, 1 normal
{
  int nombreMachine=4;
  int vitesseC=1;
  printf("Mode par défaut : \n\n");

  creationMachines(nombreMachine); //temps usinage de machines est i+1
  creationRobots();
  initialiserConvoyeur(vitesseC);
  Superviseur();

  sleep(1); //attendre que les threads soient bien en place
  affichage=1;
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
 affichage=0;

  //libererConvoyeur();
  exit(0);
}

void fairePerso(void)
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
    printf("Nombre de pièces machine %d ?\n",i);
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
  sleep(34); //TODO comment on sait quand afficher la fin ?
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
void faireDefaillance(void) {

  int choix;     // main variables
  choix = selectionChoixDefaillance();
  while(choix!=0)   //execute so long as choice is not equal to QUIT
  {
      switch(choix)
          {
              case 1 :
                  modeDeg1 = 0;
                  faireParDefaut();
                  modeDeg1 = 1;
                  break;
              case 2 :
                  modeDeg2=0;
                  faireParDefaut();
                  modeDeg2=1;
                  break;
              case 3:
                  //TODO
                  break;
              case 0:
                  break;
              default:    printf("Oups! Une erreur dans le choix du menu est survenu. ");
                          printf("Veuillez réessayer svp.\n");
          }
    choix = selectionChoixDefaillance(); /* recommence la selection */
 }
}

void menu(void) {
    int choix;     // main variables
    modeDeg1 = 1; //désactive le mode degrade 1
    modeDeg2 = 1; //désactive le mode degrade 2

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
                    faireDefaillance();
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
  //lancerSigaction(); don't works
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
