#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> //cast intptr_t
#include <unistd.h> // sleep

#define NbMachine	3 //num

void Hello(void) {
  printf("Hello, it's me lalalalalalalalalalalaaaa\n");
}

void * fonc_thread(void *k) {
  int i=*(int *)k;
  printf("Thread numero %d : mon tid est %ld\n",i,pthread_self());
  sleep(2);
  pthread_exit(NULL);
}

void creationMachines(void) {
  pthread_t pth_id[NbMachine];
  int i;
  /* creation des threads */
  for (i = 0; i < NbMachine; i++) {
    pthread_create(&pth_id[i], 0, fonc_thread, &i);
    printf("Main: thread numero %d creee: id = %ld\n",i,pth_id[i]);
  }

  for (i = 0; i < NbMachine; i++) {
    if(pthread_join(pth_id[i],NULL))
    {
      perror("ERROR; Code retour de pthread_join()");
      exit(-1);
    }
  }
}
