#ifndef _robotH_
#define _robotH_

#include "../header/machine.h"
#include "../header/superviseur.h"

pthread_cond_t robotAlim;
pthread_cond_t robotRetrait;

pthread_mutex_t alim;
pthread_mutex_t retrait;

pthread_t thread_robotAlim;
pthread_t thread_robotRetrait;


/* variable globale pour rechercher les bon elements dans les listes*/
int valRobotAlim;
int valRobotRetrait;

void * creationRobots(void);
#endif
