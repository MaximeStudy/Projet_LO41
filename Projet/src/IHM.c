#include "../header/IHM.h"
#include "sys/wait.h"
#define PAR_DEFAUT 1
#define PERSONNALISE 2
#define DEFAILLANCE 3
#define QUIT 4


void fonctionPrevenirAffichage()
{
  /* Dire à l'affichage que j'ai modifié qqch d'important*/
  pthread_mutex_lock(&mutAffichage);
  pthread_cond_signal(&condAffichage);
  pthread_mutex_unlock(&mutAffichage);
}

void traitementSIGINTmenu(int num){
  if(num!=SIGINT) printf("Echec");
  return;
}

void traitementSIGINTfils(int num){
  if(num!=SIGINT) printf("Echec");
  killThreads();
  return;
}

void * fonc_afficher()
{
      while(1)
      {
        pthread_cond_wait(&condAffichage, &mutAffichage);
        printf("*****************************************\n");
        int i;
        int j=0;

        if(conv[0].num!=-1 && pieceRobotAlim->num!=-1)
        {
          printf("conv[%2d] : %5d  %5d (Alim)\n",0, conv[0].num, pieceRobotAlim->num);
        }
        else if (conv[0].num!=-1 && pieceRobotAlim->num==-1) {
          printf("conv[%2d] : %5d  %5s (Alim)\n",0, conv[0].num, "vide");
        }
        else if (conv[0].num==-1 && pieceRobotAlim->num==-1) {
          printf("conv[%2d] : %5s  %5s (Alim)\n",0, "vide", "vide");
        }
        else  {
          printf("conv[%2d] : %5s  %5d (Alim)\n",0, "vide", pieceRobotAlim->num);
        }

        for(i=1;i<tailleConv-2;i++)
        {
	    if((i%2==0))
            {
              if(conv[i].num!=-1 && maListeMachine[j]->piece.num!=-1)
              {
                printf("conv[%2d] : %5d  %5d M[%2d]\n",i, conv[i].num, maListeMachine[j]->piece.num,j);
              }
              else if (conv[i].num!=-1 && maListeMachine[j]->piece.num==-1) {
                printf("conv[%2d] : %5d  %5s M[%2d]\n",i, conv[i].num, "vide",j);
              }
              else if (conv[i].num==-1 && maListeMachine[j]->piece.num==-1) {
                printf("conv[%2d] : %5s  %5s M[%2d]\n",i, "vide", "vide",j);
              }
              else {
                printf("conv[%2d] : %5s  %5d M[%2d]\n",i, "vide", maListeMachine[j]->piece.num,j);
              }
              j=j+1;
            }
            else
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

       if(conv[tailleConv-2].num!=-1)
        {
          printf("conv[%2d] : %5d \n",i, conv[tailleConv-2].num);
        }
        else {
          printf("conv[%2d] : %5s \n",i,"vide");
        }

        if(conv[tailleConv-1].num!=-1 && pieceRobotRetrait.num!=-1)
        {
          printf("conv[%2d] : %5d  %5d (Retr)\n",tailleConv-1, conv[tailleConv-1].num, pieceRobotRetrait.num);
        }
        else if (conv[tailleConv-1].num!=-1 && pieceRobotRetrait.num==-1) {
          printf("conv[%2d] : %5d  %5s (Retr)\n",tailleConv-1, conv[tailleConv-1].num, "vide");
        }
        else if (conv[tailleConv-1].num==-1 && pieceRobotRetrait.num==-1) {
          printf("conv[%2d] : %5s  %5s (Retr)\n",tailleConv-1, "vide", "vide");
        }
        else {
          printf("conv[%2d] : %5s  %5d (Retr)\n",tailleConv-1, "vide", pieceRobotRetrait.num);
        }
      }
}

void lancerThreadAfficheur()
{
  pthread_create(&thread_afficheur, &thread_attr, fonc_afficher, NULL);
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
        scanf("%*s"); /* On jette tous ce qu'il cause probleme au scanf */
        printf("-> Mauvais format (il faut un entier). Essayez à nouveau! <-\n");
        input_nombre(number); // recommence
    }
    return 0;
}

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
  fflush(stdout);
  option = getchar();
  while(getchar() != '\n');
  res = option-48;
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
pid_t pid;

if ((pid = fork()) == 0){

    actionINT.sa_handler=traitementSIGINTfils;
    sigaction(SIGINT, &actionINT, NULL);

	  int nombreMachine=4;
	  int vitesseC=0;
	  printf("Mode par défaut : \n\n");

	  pthread_mutex_lock(&mtx_menu);
	  creationMachines(nombreMachine); //temps usinage de machines est i+1
	  creationRobots();
	  initialiserConvoyeur(vitesseC);
	  Superviseur();

	  pthread_mutex_unlock(&mtx_menu);
	  int i;
	  sleep(1);
	  
	  pthread_mutex_lock(&mtx_menu);
	  pthread_mutex_unlock(&mtx_menu);
	  
	  lancerThreadAfficheur();

	  pthread_mutex_lock(&MitSurRobotAlim);
	  for(i=0;i<8;i++)
	  {
	       creerPiece(i%4);
	  }
	  pthread_mutex_unlock(&MitSurRobotAlim);


	  pthread_cond_wait(&Cmenu,&mtx_menu);
	  pthread_mutex_unlock(&mtx_menu);

	  for(i=0;i<NbMachine;i++)
	  {
	    printf("\nList %d\n",i);
	    afficherListe(maListeMachine[i]->listeAttente);
	  }
	  killThreads();
  }
  else {
    int status;
    /* attente terminaison fils*/
  	waitpid(pid,&status,0);
  }
}



void fairePerso(void)
{
  pid_t pid;
  if ((pid = fork()) == 0){
    actionINT.sa_handler=traitementSIGINTfils;
    sigaction(SIGINT, &actionINT, NULL);

	  int nombreMachine;
	  int vitesseC;
	  printf("Menu personnalisé : \n");
	  printf("Vitesse du convoyeur ? (vitesse décroissante)\n");
	  input_nombre(&vitesseC);

	  printf("Nombre de machine total ?\n");
	  input_nombre(&nombreMachine);

	  int i;
	  int j;
	  int tab[nombreMachine];
	  for(i=0;i<nombreMachine;i++){
		  printf("Nombre de pièces machine %d ?\n",i);
		  input_nombre(tab+i);
	  }
	  pthread_mutex_lock(&mtx_menu);
	  creationMachines(nombreMachine); //temps usinage de machines est i+1
	  creationRobots();
	  initialiserConvoyeur(vitesseC);
	  Superviseur();

	  pthread_mutex_unlock(&mtx_menu);
	  
	  usleep(50);
	  
	  pthread_mutex_lock(&mtx_menu);
	  pthread_mutex_unlock(&mtx_menu);

	  lancerThreadAfficheur();

	  pthread_mutex_lock(&MitSurRobotAlim);
	  for(i=0;i<nombreMachine;i++){
	  	for(j=0;j<tab[i];j++){
	  		creerPiece(i);
	  	}
	  }
	  pthread_mutex_unlock(&MitSurRobotAlim);
	   /* Creation convoyeur */

	  pthread_cond_wait(&Cmenu,&mtx_menu);
	  pthread_mutex_unlock(&mtx_menu);

	  /*for(i=0;i<NbMachine;i++){
		  printf("\nList %d\n",i);
		  afficherListe(maListeMachine[i]->listeAttente);
	  }
 	  killThreads();*/
  }
  else {
    int status;
    waitpid(pid,&status,0);
  }
}
void faireDefaillance(void) {

  int choix;     // main variables
  choix = selectionChoixDefaillance();
  while(choix!=5)
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

	if (choix >=1 && choix <= 5) choix =5;
	else choix = selectionChoixDefaillance(); /* recommence la selection */
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


void lancerIHM(void) {
  actionINT.sa_handler=traitementSIGINTmenu;
  sigaction(SIGINT, &actionINT, NULL);
  pthread_mutex_init(&mtx_menu,NULL);
  pthread_cond_init(&Cmenu,NULL);
  menu();
}
