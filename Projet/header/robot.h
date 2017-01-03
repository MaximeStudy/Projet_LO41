#ifndef _robotH_
#define _robotH_

#include "../header/machine.h"
#include "../header/superviseur.h"
#include "../header/IHM.h"

//pthread_cond_t condAlim;
//pthread_cond_t condRetrait;
pthread_cond_t condPose;

pthread_t thread_robotAlim;
pthread_t thread_robotRetrait;

/* variable globale pour rechercher les bon elements dans les listes*/
//int valRobotAlim;
//int valRobotRetrait;
int nbAttente;
void * creationRobots(void);

#endif
