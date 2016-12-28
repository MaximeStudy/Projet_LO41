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

struct sigaction actionTSTP;
struct sigaction actionCONT;

void lancerIHM(void);

#endif
