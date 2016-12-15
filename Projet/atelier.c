#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> //cast intptr_t
#include <unistd.h> // sleep

#define NbMachine	3 //num
int tabSleepMachine[NbMachine]= {5, 6, 8};



void * fonc_machine(void * arg) {
int* num=(int*)arg;
  while(1) {
    printf("Thread numero %d : mon tid est %ld\n",*num,pthread_self());
    sleep(tabSleepMachine[*num]);
  }
  pthread_exit(NULL);
}

void creationMachines(void) {
  pthread_t pth_id[NbMachine];
  pthread_attr_t thread_attr;
  int i ,id[NbMachine]; //c'est mieux de passer un pointeur différents pour chaque thread

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
    id[i]=i;
    pthread_create(&pth_id[i], &thread_attr, fonc_machine, &id[i]);
    printf("Main: thread numero %d creee: id = %ld\n",id[i],pth_id[i]);
  }

  /*for (i = 0; i < NbMachine; i++) {
    if(pthread_join(pth_id[i],NULL))
    {
      perror("ERROR; Code retour de pthread_join()");
      exit(-1);
    }
  } */
}
