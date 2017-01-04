#ifndef _machineH_
#define _machineH_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> //cast intptr_t
#include <unistd.h> // sleep

//#define NbMachine	4 //num
int NbMachine;
struct piece {
  int num;
  int ope; //operation d'usinage
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
  piece piece;
  pthread_cond_t dormir;
  pthread_cond_t attendre;
  pthread_mutex_t mutMachine;
};

typedef struct machine machine;
typedef machine* listeMachine;
listeMachine * maListeMachine;

pthread_attr_t thread_attr;

void creationMachines(int nb);

#endif
