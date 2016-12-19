#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> //cast intptr_t
#include <unistd.h> // sleep
#include "../header/machine.h"


void * fonc_machine(void * arg) {
  machine * ma=(machine *)arg;
  while(1) {
    printf("Thread numero %d : mon tid est %ld\n",ma->numMachine, pthread_self());
    sleep(ma->sleepTime);
  }
  pthread_exit(NULL);
}

void creationMachines(void) {
  pthread_attr_t thread_attr;
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
    pthread_create(&(nouvelleMachine->thread_id), &thread_attr, fonc_machine, nouvelleMachine);
    printf("Main: thread numero %d creee: id = %ld\n",nouvelleMachine->numMachine,(nouvelleMachine->thread_id));
  }
}
