#include "../header/machine.h"
#include "../header/robot.h"
#include "../header/convoyeur.h"

void * fonc_machine(void * arg) {
  machine * ma=(machine *)arg;
  sleep(2);
  // printf("num machine : %d\n",ma->numMachine);
  // printf("Ope : %d\n",ma->ope);
  // printf("tpsUsinage : %d\n\n",ma->tpsUsinage);
  int indexConv = (int)(2*ma->numMachine+2);
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
    piece p;
    while (1){
	pthread_cond_wait(&condPose,&mutexAlim);
	if (conv[indexConv].num != -1 && conv[indexConv].estUsine == 0 && conv[indexConv].ope == ma->ope){
	
	    p = retirerPieceConvoyeur(indexConv);
	    break;
	}
    }

    printf("%d\n",p.num);
    //sleep(ma->tpsUsinage);
    pthread_mutex_unlock(&mutexAlim);

  }
  pthread_exit(NULL);
}

void creationMachines(void) {
  int i;
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

    nouvelleMachine->numMachine=i;
    nouvelleMachine->tpsUsinage=i+1;
    nouvelleMachine->ope=i;
    nouvelleMachine->listeAttente=NULL;
    nouvelleMachine->etat=0;
    maListeMachine[i]=nouvelleMachine;

    pthread_create(&(maListeMachine[i]->thread_id), &thread_attr, fonc_machine, maListeMachine[i]);
    printf("Main: thread machine %d creee: id = %ld\n",maListeMachine[i]->numMachine,(maListeMachine[i]->thread_id));
  }
}
