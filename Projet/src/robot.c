#include "../header/include.h"
#include "../header/robot.h"
#include "../header/convoyeur.h"

void * fonc_robotAlim() {

	pthread_mutex_lock(&mtx_menu);
	pthread_mutex_unlock(&mtx_menu);
	while(1){
		pthread_cond_wait(&RobotAlim,&mutexAlim); //on attend de se faire réveiller par l'un des suivi machine
		pthread_mutex_unlock(&mutexAlim); //on débloque le mutex
		int numMachine = pieceRobotAlim->ope;

		pthread_mutex_lock(&mutexAlim);
		pthread_cond_signal(&RobotSuiviAlim); //on averti le suivi du robot qu'on a fini
		pthread_mutex_unlock(&mutexAlim);

		//boucle pour poser la piece sur le convoyeur en position 0
		if(modeDeg1==0)
			sleep(21);
		while (1){
			pthread_cond_wait(&condPose,&mutexConvoyeur); //on attend d'être sur un tournant pair pour regarder
			if (conv[0].num == -1){
				ajouterPieceConvoyeur(0,*pieceRobotAlim);
				pieceRobotAlim=pieceVideConv;
				pthread_mutex_unlock(&mutexConvoyeur);
				break;
			}

			pthread_mutex_unlock(&mutexConvoyeur);//on débloque le mutex
		}

		pthread_mutex_lock(&mutexAlim);
		pthread_cond_signal(&RobotSuiviAlim); //on averti le suivi du robot qu'on a fini

		pthread_cond_wait(&RobotAlim,&mutexAlim); //synchro pour que ça termine pas avant
		pthread_mutex_unlock(&mutexAlim); //on débloque le mutex
		pieceRobotAlim=pieceVideConv; // on a livré le piece
		fonctionPrevenirAffichage();
		pthread_mutex_unlock(&(maListeMachine[numMachine]->mutMachine));
		pthread_cond_signal(&(maListeMachine[numMachine]->dormir)); //on previent le suivi de machine qu'on a posé la piece pour qu'il libère le mutex et qu'un autre thread de suivi puisse poser sa piece.
		pthread_mutex_unlock(&(maListeMachine[numMachine]->mutMachine)); //on libere le mutex

	}
	pthread_exit(NULL);
}


void * fonc_robotRetrait() {
	pthread_mutex_lock(&mtx_menu);
	pthread_mutex_unlock(&mtx_menu);
	while(1)
	{
		pthread_cond_wait(&RobotRetrait,&mutexRetrait);//on se fait reveiller par l'un des thread suivi machine
		pthread_mutex_unlock(&mutexRetrait);

		pthread_mutex_lock(&mutexRetrait);
		pthread_cond_signal(&RobotSuiviRetrait); //on averti le suivi du robot qu'on commence
		pthread_mutex_unlock(&mutexRetrait);
		if(modeDeg4==0)
			sleep(31);
		int op;
		while (1){ //on lance la boucle pour retirer la piece
			pthread_cond_wait(&condPose/*2*/,&mutexConvoyeur ); //on attend que ce soit impair
			if (conv[(tailleConv-1)].num != -1){ //impair donc piece usiné

				pieceRobotRetrait = retirerPieceConvoyeur(tailleConv-1); //on retire la piece
				fonctionPrevenirAffichage();
				pthread_mutex_unlock(&mutexConvoyeur);
				op=pieceRobotRetrait.ope;
				//free(p);
				break;
			}
			pthread_mutex_unlock(&mutexConvoyeur);
		}

		pthread_mutex_lock(&mutexRetrait);
		pthread_cond_signal(&RobotSuiviRetrait); //on averti le suivi du robot qu'on a fini

		pthread_cond_wait(&RobotRetrait,&mutexRetrait); //synchro pour que ça termine pas avant
		pthread_mutex_unlock(&mutexRetrait); //on débloque le mutex

		sleep(1); //pour avoir le temps de voir la piece sur l'affichage

		pieceRobotRetrait=pieceVideSortie; // on a livré le piece
		fonctionPrevenirAffichage();
		//on retourne dormir et on unlock mutexRetrait.
		pthread_mutex_lock(&(maListeMachine[op]->mutMachine));
		pthread_cond_signal(&(maListeMachine[op]->dormir));
		pthread_mutex_unlock(&(maListeMachine[op]->mutMachine));


	}
	pthread_exit(NULL);
}

void  creationRobots(void) {
  /* creation des threads */
  pthread_cond_init(&condPose,NULL);

  pieceRobotAlim=pieceVideConv;
  pieceVideSortie.num=-1;
  pieceVideSortie.ope=-1;
  pieceRobotRetrait=pieceVideSortie;

  pthread_create(&thread_robotAlim, &thread_attr, fonc_robotAlim, NULL);
  pthread_create(&thread_robotRetrait, &thread_attr, fonc_robotRetrait, NULL);
}
