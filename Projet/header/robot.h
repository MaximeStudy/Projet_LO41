#ifndef _robotH_
#define _robotH_

#include "../header/machine.h"
#include "../header/superviseur.h"
#include "../header/IHM.h"

pthread_cond_t condPose;

pthread_t thread_robotAlim;
pthread_t thread_robotRetrait;

void creationRobots(void);

#endif
