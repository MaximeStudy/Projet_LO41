#include "../header/include.h"
#include "../header/robot.h"
#include "../header/convoyeur.h"


//version 1
/*void * fonc_robotAlim(void * arg) {
  int i;
  while(1)
  {
    pthread_mutex_lock(&mutexAlim);
    //Si aucune machine n'a besoin de piece
    if(nbAttente==0)
    {
      pthread_cond_wait(&condAlim,&mutexAlim);
    }
    for(i=0;i<NbMachine;i++)
    {
        //elle attend une reponse du robot (oblige car sinon on peu perdre des messages)
        if(maListeMachine[i]->etat==1 && maListeMachine[i]->defaillant==0)
        {
          //printf("Hello je suis le robot d'alim et %d veux que je mette une piece\n",i);
          pthread_cond_signal(&maListeMachine[i]->attendre);
          piece *p=recupererElementEnTete(maListeMachine[i]->listeAttente);
          maListeMachine[i]->listeAttente=supprimerElementEnTete(maListeMachine[i]->listeAttente);
          nbAttente--;
          maListeMachine[i]->etat=0;
          //afficherListe(maListeMachine[i]->listeAttente);

	while ( 1){
		pthread_cond_wait(&condPose,&mutexAlim);
		if (conv[0].num == -1){
		    ajouterPieceConvoyeur(0,*p);
		    pthread_cond_signal(&maListeMachine[i]->attendre);
		    break;
		}
	}
	break;
        }
    }
    pthread_mutex_unlock(&mutexAlim);
  }
  pthread_exit(NULL);
}*/

void * fonc_robotAlim(void * arg) {

	while(1){
		pthread_cond_wait(&RobotAlim,&mutexAlim); //on attend de se faire réveiller par l'un des suivi machine
		pthread_mutex_unlock(&mutexAlim); //on débloque le mutex
		int numMachine = pieceRobotAlim->ope;

		pthread_mutex_lock(&mutexSuiviAlim);
		pthread_cond_signal(&RobotSuiviAlim); //on averti le suivi du robot qu'on a fini
		pthread_mutex_unlock(&mutexSuiviAlim);

		//boucle pour poser la piece sur le convoyeur en position 0
		while (1){
			pthread_cond_wait(&condPose,&mutexConvoyeur); //on attend d'être sur un tournant pair pour regarder
			if (conv[0].num == -1){
				ajouterPieceConvoyeur(0,*pieceRobotAlim);
				pthread_mutex_unlock(&mutexConvoyeur);
				break;
			}
			pthread_mutex_unlock(&mutexConvoyeur);//on débloque le mutex
		}

		pthread_mutex_lock(&mutexSuiviAlim);
		pthread_cond_signal(&RobotSuiviAlim); //on averti le suivi du robot qu'on a fini
		pthread_mutex_unlock(&mutexSuiviAlim);

		pthread_mutex_unlock(&(maListeMachine[numMachine]->mutMachine));
		pthread_cond_signal(&(maListeMachine[numMachine]->dormir)); //on previent le suivi de machine qu'on a posé la piece pour qu'il libère le mutex et qu'un autre thread de suivi puisse poser sa piece.
		pthread_mutex_unlock(&(maListeMachine[numMachine]->mutMachine)); //on libere le mutex
	}
	pthread_exit(NULL);
}


void * fonc_robotRetrait(void * arg) {
	piece p;
	while(1)
	{
		pthread_cond_wait(&RobotRetrait,&mutexRetrait);//on se fait reveiller par l'un des thread suivi machine
		pthread_mutex_unlock(&mutexRetrait);

		pthread_mutex_lock(&mutexSuiviRetrait);
		pthread_cond_signal(&RobotSuiviRetrait); //on averti le suivi du robot qu'on a fini
		pthread_mutex_unlock(&mutexSuiviRetrait);

		while (1){ //on lance la boucle pour retirer la piece
			pthread_cond_wait(&condPose2,&mutexConvoyeur); //on attend que ce soit impair
			if (conv[(tailleConv-1)].num != -1){ //impair donc piece usiné
				p = retirerPieceConvoyeur(tailleConv-1); //on retire la piece
				printf("ROBOT DE RETRAIT Piece retiré ------------------------%d\n",p.num);
				pthread_mutex_unlock(&mutexConvoyeur);
				break;
			}
			pthread_mutex_unlock(&mutexConvoyeur);
		}

		pthread_mutex_lock(&mutexSuiviRetrait);
		pthread_cond_signal(&RobotSuiviRetrait); //on averti le suivi du robot qu'on a fini
		pthread_mutex_unlock(&mutexSuiviRetrait);

		//on retourne dormir et on unlock mutexRetrait.
		pthread_mutex_lock(&(maListeMachine[p.ope]->mutMachine));
		pthread_cond_signal(&(maListeMachine[p.ope]->dormir));
		pthread_mutex_unlock(&(maListeMachine[p.ope]->mutMachine));

	}
	pthread_exit(NULL);
}

void  * creationRobots(void) {
  /* creation des threads */
  nbAttente=0;
  //pthread_mutex_init(&mutexAlim,NULL);
  //pthread_mutex_init(&mutexRetrait,NULL);

  //pthread_cond_init(&condAlim,NULL);
  //pthread_cond_init(&condRetrait,NULL);
  pthread_cond_init(&condPose,NULL);
  pthread_cond_init(&condPose2,NULL);
  pthread_create(&thread_robotAlim, &thread_attr, fonc_robotAlim, NULL);
  pthread_create(&thread_robotRetrait, &thread_attr, fonc_robotRetrait, NULL);
}
