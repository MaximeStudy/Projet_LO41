
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> //cast intptr_t
#include <unistd.h> // sleep

#define NbMachine	3 //num

struct machine {
  pthread_t thread_id;
  int numMachine;
  //int ope;
  int sleepTime;
  int defaillant;
};

typedef struct machine machine;
typedef machine* listeMachine;
listeMachine * maListeMachine;

pthread_attr_t thread_attr;

void creationMachines(void);
