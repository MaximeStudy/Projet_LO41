#include "../header/include.h"
#include "../header/robot.h"

void * fonc_robotAlim(void * arg) {
  int i;
  while(1)
  {
    pthread_mutex_lock(&mutexAlim);
    //Si aucune machine n'a besoin de piece
    if(nbAttente==0)
    {
      pthread_cond_wait(&condAlim,&mutexAlim);
    }
    for(i=0;i<NbMachine;i++)
    {
        //elle attend une reponse du robot (oblige car sinon on peu perdre des messages)
        if(maListeMachine[i]->etat==1)
        {
          printf("Hello je suis le robot d'alim et %d veux que je mette une piece\n",i);
          pthread_cond_signal(&maListeMachine[i]->attendre);
          piece *p=recupererElementEnTete(maListeMachine[i]->listeAttente);
          maListeMachine[i]->listeAttente=supprimerElementEnTete(maListeMachine[i]->listeAttente);
          nbAttente--;
          maListeMachine[i]->etat=0;
          afficherListe(maListeMachine[i]->listeAttente);
          break;
        }
    }
    pthread_mutex_unlock(&mutexAlim);
  }
  pthread_exit(NULL);
}


void * fonc_robotRetrait(void * arg) {
  while(1)
  {
    pthread_mutex_lock(&mutexRetrait);
    pthread_cond_wait(&condRetrait,&mutexRetrait);
    pthread_mutex_unlock(&mutexRetrait);
  }
  pthread_exit(NULL);
}

void  * creationRobots(void) {
  /* creation des threads */
  nbAttente=0;
  pthread_mutex_init(&mutexAlim,NULL);
  pthread_mutex_init(&mutexRetrait,NULL);

  pthread_cond_init(&condAlim,NULL);
  pthread_cond_init(&condRetrait,NULL);
  pthread_create(&thread_robotAlim, &thread_attr, fonc_robotAlim, NULL);
  pthread_create(&thread_robotRetrait, &thread_attr, fonc_robotRetrait, NULL);
}
