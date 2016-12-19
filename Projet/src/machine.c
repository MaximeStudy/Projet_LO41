

#include "../header/machine.h"


void * fonc_machine(void * arg) {
  machine * ma=(machine *)arg;
  while(1) {
    //printf("Thread numero %d : mon tid est %ld\n",ma->numMachine, pthread_self());

    sleep(ma->sleepTime);
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
    nouvelleMachine->numMachine=i;
    nouvelleMachine->sleepTime=i+2;
    maListeMachine[i]=nouvelleMachine;
    pthread_create(&(maListeMachine[i]->thread_id), &thread_attr, fonc_machine, maListeMachine[i]);
    printf("Main: thread numero %d creee: id = %ld\n",maListeMachine[i]->numMachine,(maListeMachine[i]->thread_id));
  }
}
