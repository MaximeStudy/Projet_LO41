#include "../header/machine.h"
#include "../header/robot.h"
#include "../header/convoyeur.h"

//Version 1
/*void * fonc_machine(void * arg) {
  machine * ma=(machine *)arg;
  sleep(2);
  // printf("num machine : %d\n",ma->numMachine);
  // printf("Ope : %d\n",ma->ope);
  // printf("tpsUsinage : %d\n\n",ma->tpsUsinage);
  int indexConv = (int)(2*ma->numMachine+1);
  piece p;
  while(1) {
    pthread_mutex_lock(&mutexAlim);
    if(ma->listeAttente==NULL) {
        //la liste est nul -> attendre
        //printf("La machine %d dort car il y a pas de piece en attente\n",mutexAlim);
        pthread_cond_wait(&ma->dormir,&mutexAlim);
    }


    ma->etat=1;
    pthread_cond_signal(&condAlim);
    //printf("nb attente %d\n",nbAttente);
    nbAttente=nbAttente+1;

    //previent le robot d'alim

    //sleep(1);



    //attente de reponse du robot d'alim

    pthread_cond_wait(&ma->attendre,&mutexAlim);
    while (1){
		pthread_cond_wait(&condPose,&mutexAlim);
		if (conv[indexConv].num != -1 && conv[indexConv].estUsine == 0 && conv[indexConv].ope == ma->ope){

			p = retirerPieceConvoyeur(indexConv);
			break;
		}
    }

    pthread_cond_signal(&(ma->dormir));

	printf("%d DEBUT\n",(ma->numMachine));

    pthread_mutex_unlock(&mutexAlim);

    sleep(ma->tpsUsinage+10);

	pthread_cond_signal(&(ma->dormir));

    p.estUsine = 1;

    //printf("2.machine n°%d\n",(ma->numMachine));

	pthread_cond_wait(&(ma->attendre),&(ma->mutMachine));

	printf("%d FIN\n",(ma->numMachine));

    pthread_mutex_lock(&mutexAlim);

	while (1){
		pthread_cond_wait(&condPose2,&mutexAlim);
		if (conv[indexConv+1].num == -1){
		    ajouterPieceConvoyeur(indexConv+1,p);
            pthread_cond_signal(&condRetrait);
		    break;
		}
	}

    ma->etat=0;

    pthread_mutex_unlock(&mutexAlim);

  }
  pthread_exit(NULL);
}*/

//version 2
void * fonc_machine(void * arg) {
	machine * ma=(machine *)arg;
	piece p;
	int indexConv = (int)(2*ma->numMachine+2); //case sur laquelle la machine prend la piece (sur le convoyeur)
	while(1){
		pthread_cond_wait(&(ma->attendre), &(ma->mutMachine)); //on attend qu'une piece soit arrivé au superviseur
		pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex
		//printf("Machine se reveille %d\n", ma->numMachine);
		while (1){
			pthread_cond_wait(&condPose,&mutexConvoyeur); //on attend d'être sur un tournant pair pour regarder
			if (conv[indexConv].num != -1 && conv[indexConv].ope == ma->ope){
				//printf("Machine PREND LA PIECE %d\n", ma->numMachine);
				p = retirerPieceConvoyeur(indexConv); //on retire la piece
				pthread_mutex_unlock(&mutexConvoyeur);
				break;
			}
			pthread_mutex_unlock(&mutexConvoyeur);//on débloque le mutex
    		}
		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->dormir)); //on previent le thread suivi de la machine que la machine va traiter la piece.
		pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex

		//printf("Machine DEMARRE USINAGE %d\n", ma->numMachine);

		sleep(ma->tpsUsinage+10); //on execute le traitement sur la machine

		//printf("Machine FINI USINAGE %d\n", ma->numMachine);

		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->dormir)); //on signal qu'on a terminé le traitement sur la piece à suivi machine
		pthread_mutex_unlock(&(ma->mutMachine));

		p.estUsine = 1; //on dit à la piece qu'elle est unisé.

		//printf("Machine ATTEND REPONSE DE SUIVI %d\n", ma->numMachine);

		pthread_cond_wait(&(ma->attendre),&(ma->mutMachine)); //on attend une réponse de la part du suivi
		pthread_mutex_unlock(&(ma->mutMachine));
		//printf("Machine REPONSE RECU DEMARRE PLACEMENT %d\n", ma->numMachine);

		while (1){
			pthread_cond_wait(&condPose2,&mutexConvoyeur);//on attend d'être sur un tournant impair pour regarder
			if (conv[indexConv+1].num == -1){
				//printf("Machine POSE SUR CONVOYEUR%d\n", ma->numMachine);
				ajouterPieceConvoyeur(indexConv+1,p); //on pose la piece
				pthread_mutex_unlock(&mutexConvoyeur);
				break;
			}
			pthread_mutex_unlock(&mutexConvoyeur);//on débloque le mutex
		}
    		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->dormir)); //on signal qu'on a posé la piece sur le convoyeur à SuiviMachine
		pthread_mutex_unlock(&(ma->mutMachine));

	}

	pthread_exit(NULL);

}

void creationMachines(void) {
  int i;
	NbMachine=4;
  maListeMachine=malloc(NbMachine*sizeof(machine));
  //permet d'eviter la fuite memoire
  if(pthread_attr_init(&thread_attr)!=0) {
      printf("pthrad_attr_init error");
      exit(1);
  }
  if(pthread_attr_setdetachstate (&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
      printf("pthread_attr_setdetachestate error");
      exit(1);
  }
  /* creation des threads */
  for (i = 0; i < NbMachine; i++) {
    machine * nouvelleMachine = malloc(sizeof(machine));
    //pthread_mutex_init(&nouvelleMachine->mutexMachine,NULL);
    pthread_cond_init(&nouvelleMachine->dormir,NULL);
    pthread_cond_init(&nouvelleMachine->attendre,NULL);
    //pthread_mutex_init(&nouvelleMachine->mutMachineDefaillance,NULL);
    pthread_mutex_init(&nouvelleMachine->mutMachine,NULL);

    nouvelleMachine->numMachine=i;
    nouvelleMachine->tpsUsinage=i+1;
    nouvelleMachine->ope=i;
    nouvelleMachine->listeAttente=NULL;
    //nouvelleMachine->etat=0;
    nouvelleMachine->defaillant=0;
    maListeMachine[i]=nouvelleMachine;

    pthread_create(&(maListeMachine[i]->thread_id), &thread_attr, fonc_machine, maListeMachine[i]);
    printf("Main: thread machine %d creee: id = %ld\n",maListeMachine[i]->numMachine,(maListeMachine[i]->thread_id));
  }
}
