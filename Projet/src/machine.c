

#include "../header/machine.h"


void * fonc_machine(void * arg) {
  machine * ma=(machine *)arg;
  while(1) {
    pthread_mutex_lock(&ma->mutexMachine);
    if(ma->listeAttente==NULL) {
        //la liste est nul -> attendre
        printf("La machine %d dort car il y a pas de piece en attente\n",ma->numMachine);
        pthread_cond_wait(&ma->dormir,&ma->mutexMachine);
    }
    else {

    }
    sleep(ma->tpsUsinage);
    pthread_mutex_unlock(&ma->mutexMachine);
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
    pthread_mutex_init(&nouvelleMachine->mutexMachine,NULL);
    pthread_cond_init(&nouvelleMachine->dormir,NULL);
    pthread_mutex_lock(&nouvelleMachine->mutexMachine);
    nouvelleMachine->numMachine=i;
    nouvelleMachine->tpsUsinage=i+2;
    nouvelleMachine->ope=i;
    nouvelleMachine->listeAttente=NULL;
    maListeMachine[i]=nouvelleMachine;
    pthread_mutex_unlock(&nouvelleMachine->mutexMachine);

    pthread_create(&(maListeMachine[i]->thread_id), &thread_attr, fonc_machine, maListeMachine[i]);
    printf("Main: thread numero %d creee: id = %ld\n",maListeMachine[i]->numMachine,(maListeMachine[i]->thread_id));
  }
}
