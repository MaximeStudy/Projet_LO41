#ifndef _IHMH_
#define _IHMH_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "../header/machine.h"
#include "../header/robot.h"
#include "../header/superviseur.h"
#include "../header/convoyeur.h"

struct sigaction actionINT;

pthread_cond_t Cmenu;
pthread_mutex_t mtx_menu;

void lancerIHM(void);

#endif
