#include "../header/include.h"
#include "../header/robot.h"

void * fonc_robotAlim(void * arg) {
  while(1)
  {
    pthread_mutex_lock(&alim);
    pthread_cond_wait(&robotAlim,&alim);
    piece *p=recupererElementEnTete(maListeMachine[valRobotAlim]->listeAttente);
    maListeMachine[valRobotAlim]->listeAttente=supprimerElementEnTete(maListeMachine[valRobotAlim]->listeAttente);
    pthread_mutex_unlock(&alim);
  }
  pthread_exit(NULL);
}

void * fonc_robotRetrait(void * arg) {
  while(1)
  {
    pthread_mutex_lock(&retrait);
    pthread_cond_wait(&robotRetrait,&retrait);
    pthread_mutex_unlock(&retrait);

  }
  pthread_exit(NULL);
}

void  * creationRobots(void) {
  /* creation des threads */
  pthread_create(&robotAlim, &thread_attr, fonc_robotAlim, NULL);
  pthread_create(&robotRetrait, &thread_attr, fonc_robotRetrait, NULL);
}
