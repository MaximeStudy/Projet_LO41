#include "../header/IHM.h"
#define PAR_DEFAUT 1
#define PERSONNALISE 2
#define DEFAILLANCE 3
#define QUIT 4


int selectionChoixDefaillance(void)
{
  char option;
  int res;
  printf("\nMenu de sélection de défaillances :\n\n");
  printf("\t1.\tRobot alimentation : Placement sur convoyeur\n");
  printf("\t2.\tMachine : Retirer piece du convoyeur\n");
  printf("\t3.\tMachine : travail long \n");
  printf("\t4.\tRobot Retrait : retrait du convoyeur\n");
  printf("\t5.\tQuit\n\n");
  printf("Faite votre choix : ");
  option = getchar();
  while(getchar() != '\n');
    res = option-48;
  while( (1 > res)  || (res > 5))
  {
    fflush(stdin);                    /* clear bad data from buffer */
    printf("La selection n'est pas valide. Essayez à nouveau.\n\n");
    printf("Votre choix ? ");
    option = getchar();
    while(getchar() != '\n');
    res = option-48; //code ascii
  }
  return res;
}

int selectionChoix(void)
{
  char option;
  int res;
  printf("\nMenu de sélection :\n\n");
  printf("1.\tPar défaut\n");
  printf("2.\tMode personnalisé\n");
  printf("3.\tMode défaillance\n");
  printf("4.\tQuit\n\n");
  printf("Faite votre choix : ");
  option = getchar();
  while(getchar() != '\n');
  res = option-48;
  printf("%d", res);
  while( (res < 1)  || (res > 4))
  {
    fflush(stdin);                    /* clear bad data from buffer */
    printf("La selection n'est pas valide. Essayez à nouveau.\n\n");
    printf("Votre choix ? ");
    option = getchar();
    while(getchar() != '\n');
    res = option-48;
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
  pthread_mutex_lock(&MitSurRobotAlim);
  for(i=0;i<8;i++)
  {
       creerPiece(i%4);
  }
  pthread_mutex_unlock(&MitSurRobotAlim);
   /* Creation convoyeur */

  //afficherConvoyeur();
  pthread_cond_wait(&Cmenu,&mtx_menu);
  pthread_mutex_unlock(&mtx_menu);

  printf("nb attente : %d",nbAttente);
  for(i=0;i<NbMachine;i++)
  {
    printf("\nList %d\n",i);
    afficherListe(maListeMachine[i]->listeAttente);
 }
 affichage=0;
 killThreads();
}

int input_nombre(int * number)
{
    printf("Nombre : ");
    if ( scanf("%d", number) == 1 ) {
      if(*number<0)
      {
        printf("-> Mauvais format (inférieur à 0), Essayez à nouveau! <-\n");
        input_nombre(number);
      }
      else
      {
        return 0;
      }
    }
    else
    {
        scanf("%*s"); /* <--this will read and discard whatever caused scanf to fail */
        printf("-> Mauvais format(il faut un entier), Essayez à nouveau! <-\n");
        input_nombre(number); // start over
    }
}

void fairePerso(void)
{
  int nombreMachine;
  int vitesseC;
  printf("Menu personnalisé : \n");
  printf("Vitesse du convoyeur ? (vitesse décroissante)\n");
  //scanf("%d", &vitesseC);
  input_nombre(&vitesseC);

  printf("Nombre de machine total ?\n");
  input_nombre(&nombreMachine);
  //scanf(" %d", &nombreMachine);



  int i;
  int j;
  int tab[nombreMachine];
  for(i=0;i<nombreMachine;i++)
  {
    printf("Nombre de pièces machine %d ?\n",i);
    //scanf("%d",tab+i); //TODO verif int dans le scan f
    input_nombre(tab+i);

  }
  creationMachines(nombreMachine); //temps usinage de machines est i+1
  creationRobots();
  initialiserConvoyeur(vitesseC);
  Superviseur();

  sleep(2); //attendre que les threads soient bien en place
  pthread_mutex_lock(&MitSurRobotAlim);
  for(i=0;i<nombreMachine;i++)
  {
    for(j=0;j<tab[i];j++)
    {
      creerPiece(i);
    }
  }
  pthread_mutex_unlock(&MitSurRobotAlim);
  affichage=1;
   /* Creation convoyeur */

  //afficherConvoyeur();
  pthread_cond_wait(&Cmenu,&mtx_menu);
  pthread_mutex_unlock(&mtx_menu);

  affichage=0;

  printf("nb attente : %d",nbAttente);
  for(i=0;i<NbMachine;i++)
  {
    printf("\nList %d\n",i);
    afficherListe(maListeMachine[i]->listeAttente);
 }
 killThreads();
}
void faireDefaillance(void) {

  int choix;     // main variables
  choix = selectionChoixDefaillance();
  while(choix!=5)   //execute so long as choice is not equal to QUIT
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
                  modeDeg3 = 0;
                  faireParDefaut();
                  modeDeg3 = 1;
                  break;
              case 4:
                  modeDeg4 = 0;
                  faireParDefaut();
                  modeDeg4 = 1;
                  break;
              case 5:
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
    modeDeg3 = 1; //désactive le mode degrade 3
    modeDeg4 = 1; //désactive le mode degrade 4

    choix = selectionChoix();   // recupere la selection de l'utilisateur

    while(1)
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

void yolo(int num){
  if(num!=SIGINT) printf("Echec");

  pthread_mutex_lock(&mtx_menu);
  pthread_cond_signal(&Cmenu);
  pthread_mutex_unlock(&mtx_menu);
  return;

}

void lancerIHM(void) {


struct sigaction sigInt;
sigInt.sa_handler=yolo;
sigaction(SIGINT, &sigInt, NULL);
  pthread_mutex_init(&mtx_menu,NULL);
  pthread_cond_init(&Cmenu,NULL);
  menu();
}
