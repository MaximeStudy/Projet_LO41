#include "../header/machine.h"
#include "../header/robot.h"
#include "../header/convoyeur.h"

void * fonc_machine(void * arg) {
	machine * ma=(machine *)arg;
	piece p;
	int indexConv = (int)(2*ma->numMachine+2); //case sur laquelle la machine prend la piece (sur le convoyeur)
	while(1){
		pthread_cond_wait(&(ma->attendre), &(ma->mutMachine)); //on attend qu'une piece soit arrivé au superviseur
		pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex

		if(modeDeg2==0 && ma->numMachine==0) //simulation du deuxieme cas de defaillance pour la machine 0
			sleep(15);

		while (1){
			pthread_cond_wait(&condPose,&mutexConvoyeur); //on attend d'être sur un tournant pair pour regarder
			if (conv[indexConv].num != -1 && conv[indexConv].ope == ma->ope){
				p = retirerPieceConvoyeur(indexConv); //on retire la piece
				pthread_mutex_unlock(&mutexConvoyeur);
				break;
			}
			pthread_mutex_unlock(&mutexConvoyeur);//on débloque le mutex
    		}
		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->dormir)); //on previent le thread suivi de la machine que la machine va traiter la piece.
		pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex

		if (modeDeg3 == 0 && ma->numMachine == 0) sleep(22);

		else sleep(ma->tpsUsinage); //on execute le traitement sur la machine

		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->dormir)); //on signal qu'on a terminé le traitement sur la piece à suivi machine
		pthread_mutex_unlock(&(ma->mutMachine));

		pthread_cond_wait(&(ma->attendre),&(ma->mutMachine)); //on attend une réponse de la part du suivi
		pthread_mutex_unlock(&(ma->mutMachine));

		while (1){
			pthread_cond_wait(&condPose/*2*/,&mutexConvoyeur);//on attend d'être sur un tournant impair pour regarder
			if (conv[indexConv+1].num == -1){
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

void creationMachines(int nb) {
  int i;
	NbMachine=nb;
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
    pthread_cond_init(&nouvelleMachine->dormir,NULL);
    pthread_cond_init(&nouvelleMachine->attendre,NULL);
    pthread_mutex_init(&nouvelleMachine->mutMachine,NULL);

    nouvelleMachine->numMachine=i;
    nouvelleMachine->tpsUsinage=i+1;
    nouvelleMachine->ope=i;
    nouvelleMachine->listeAttente=NULL;
    nouvelleMachine->defaillant=0;
    maListeMachine[i]=nouvelleMachine;

    pthread_create(&(maListeMachine[i]->thread_id), &thread_attr, fonc_machine, maListeMachine[i]);
    printf("Main: thread machine %d creee: id = %ld\n",maListeMachine[i]->numMachine,(maListeMachine[i]->thread_id));
  }
}
