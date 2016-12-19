#ifndef _machineH_
#define _machineH_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> //cast intptr_t
#include <unistd.h> // sleep

#define NbMachine	4 //num
struct piece {
  int num;
  int ope; //operation d'usinage
  int estUsine; //pour svaoir si la piece est usine (necessaire ?)
};
typedef struct piece piece;
int num; //compteur

/* Liste chainée*/
struct element
{
    piece val;
    struct element *nxt;
};
typedef struct element element;
typedef element* llist;

struct machine {
  pthread_t thread_id;
  int numMachine;
  int ope;
  int tpsUsinage;
  int defaillant;
  llist listeAttente; //chaque machine a sa liste d'attente
  pthread_mutex_t mutexMachine;
  pthread_cond_t dormir;
};

typedef struct machine machine;
typedef machine* listeMachine;
listeMachine * maListeMachine;

pthread_attr_t thread_attr;

void creationMachines(void);

#endif
