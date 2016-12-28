#include <stdlib.h>
#include <stdio.h>
#include "../header/machine.h"
#include "../header/superviseur.h"


llist ajouterEnFin(llist liste, piece valeur)
{
    /* On crée un nouvel élément */
    element* nouvelElement = malloc(sizeof(element));

    /* On assigne la valeur au nouvel élément */
    nouvelElement->val = valeur;

    /* On ajoute en fin, donc aucun élément ne va suivre */
    nouvelElement->nxt = NULL;

    if(liste == NULL)
    {
        /* Si la liste est videé il suffit de renvoyer l'élément créé */
        return nouvelElement;
    }
    else
    {
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
        element* temp=liste;
        while(temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = nouvelElement;
        return liste;
    }
}

llist supprimerElementEnTete(llist liste)
{
    if(liste != NULL)
    {
        /* Si la liste est non vide, on se prépare à renvoyer l'adresse de
        l'élément en 2ème position */
        element* aRenvoyer = liste->nxt;
        /* On libère le premier élément */
        //free(liste);
        /* On retourne le nouveau début de la liste */
        return aRenvoyer;
    }
    else
    {
        return NULL;
    }
}

piece * recupererElementEnTete(llist liste)
{
    if(liste != NULL)
    {
        /* Si la liste est non vide, on se prépare à renvoyer l'adresse de
        l'élément en 2ème position */
        piece *aRenvoyer = &(liste->val);
        /* On retourne le nouveau début de la liste */
        return aRenvoyer;
    }
    else
    {
        return NULL; //si aucun element dans la liste
    }
}

void afficherListe(llist liste)
{
    printf("*****************************************\n");
    if(liste==NULL)
    {
      printf("la liste est vide\n");
    }
    else {
      element *tmp = liste;

      /* Tant que l'on n'est pas au bout de la liste */
      while(tmp != NULL)
      {
          /* On affiche */
          printf("num   : %5d ", tmp->val.num);
          printf("Ope   : %5d ", tmp->val.ope);
          printf("Usine : %5d\n", tmp->val.estUsine);
          /* On avance d'une case */
          tmp = tmp->nxt;
      }
    }
}

llist effacerListe(llist liste)
{
    element* tmp = liste;
    element* tmpnxt;

    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)
    {
        /* On stocke l'élément suivant pour pouvoir ensuite avancer */
        tmpnxt = tmp->nxt;
        /* On efface l'élément courant */
        free(tmp);
        /* On avance d'une case */
        tmp = tmpnxt;
    }
    /* La liste est vide : on retourne NULL */
    return NULL;
}

void creerPiece(int ope)
{
  piece* nouvellePiece = malloc(sizeof(piece));
  nouvellePiece->num = num;
	printf("%d\n",nouvellePiece->num);
  nouvellePiece->ope = ope;
  nouvellePiece->estUsine =0;
  num++; //pas besoin de le proteger, il y a juste un thread qui l'incremente

  int test = 0;
  if (recupererElementEnTete(maListeMachine[ope]->listeAttente) == NULL){
  	test = 1;
  }
  maListeMachine[ope]->listeAttente=ajouterEnFin(maListeMachine[ope]->listeAttente, *nouvellePiece);

  if (test ==1){ //si la liste d'attente pour la machine était vide
  	pthread_cond_signal(&(maListeMachine[ope]->dormir)); //on previent qu'il y a une pieces
  	pthread_mutex_unlock(&(maListeMachine[ope]->mutMachine)); //on libere le mutex
  }
  printf("Hi\n" );

}

/* Suivi machine du superviseur */
//Version 1
/*void * threadSuiviMachine(void * arg) {
  machine * ma=(machine *)arg;
  struct timeval tp;
  struct timespec ts;
  int rc;
  int res;
  pthread_mutex_lock(&(ma->mutMachineDefaillance));
  while(1) {
    pthread_cond_wait(&(ma->dormir),&(ma->mutMachine));
    rc = gettimeofday(&tp, NULL);
    ts.tv_sec = tp.tv_sec;
    ts.tv_nsec = tp.tv_usec*1000;
    ts.tv_sec += 20;
    printf("%d BBBBB\n",(ma->numMachine));
    res = pthread_cond_timedwait(&(ma->dormir),&(ma->mutMachineDefaillance),&ts);
    if(res != 0){
      //pthread_mutex_unlock(&(ma->mutMachineDefaillance));
      ma->defaillant = 1;
      printf("%d echec!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",(ma->numMachine));
      break;
    }

    printf("%d OKKKKKKKKKKKKK\n",(ma->numMachine));
    sleep(2);
    pthread_cond_signal(&(ma->attendre));
  }

  pthread_mutex_unlock(&(ma->mutMachineDefaillance));
  pthread_exit(NULL);
}*/

//version 2
void * threadSuiviMachine(void * arg) {
	machine * ma=(machine *)arg;
	struct timeval tp;
	struct timespec ts;
	int rc;

	while(EnMarche == 1 && ma->defaillant == 0){

		//printf("%d avant if\n",ma->numMachine);

		if (recupererElementEnTete(ma->listeAttente) == NULL){ //si pas de piece pour cette machine
			pthread_cond_wait(&(ma->dormir),&(ma->mutMachine)); //on s'endort le temps d'en recevoir une
			pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex
		}
		//printf("%d apres if\n",ma->numMachine);
		pthread_mutex_lock(&MitSurRobotAlim); //on tente de lock pour poser la piece sur le robot d'alim
		pieceRobotAlim = recupererElementEnTete(ma->listeAttente); //on la pose
		//printf("%d apres premier lock\n",ma->numMachine);
		ma->listeAttente=supprimerElementEnTete(ma->listeAttente); //on supprime la piece de la liste d'attente
		//printf("%d on signal au robot\n",ma->numMachine);
		pthread_mutex_lock(&mutexAlim);
		pthread_cond_broadcast(&RobotAlim); //on envoie un signal au robot d'alim + son suivi pour lui dire que c'est fait
		pthread_mutex_unlock(&mutexAlim);

		//printf("%d on s'endort\n",ma->numMachine);

		pthread_cond_wait(&(ma->dormir),&(ma->mutMachine)); //on s'endort le temps que le robot fasse la manip
		pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex

		pthread_mutex_unlock(&MitSurRobotAlim); //on unlock pour que les autres thread de suivi puissent poser une piece sur le robot d'alim

		//printf("%d on se reveil et on signal a la machine\n",ma->numMachine);

		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->attendre)); //on previent la machine que la piece arrive
		pthread_mutex_unlock(&(ma->mutMachine));

		//printf("%d on attend reponse de machine \n",ma->numMachine);

		//on attend que la machine recoive la piece et lance le traitement



		rc = gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec*1000;
		ts.tv_sec += 50;
		//on s'endort le temps que la piece soit récupéré par la machine.
		//mais si le temps dépasse les 20 secondes de traitement (ts), le SYSTEME passe en défaillant
		if(pthread_cond_timedwait(&(ma->dormir),&(ma->mutMachine),&ts) != 0){
			pthread_mutex_unlock(&(ma->mutMachine));
			ma->defaillant = 1;
			printf("%d ECHEC TIMEDWAIT\n",ma->numMachine);
			EnMarche = 0;
			break;
		}
		pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex


		//printf("%d on demarre timedwait\n",ma->numMachine);

		rc = gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec*1000;
		ts.tv_sec += 20;
		//on s'endort le temps que la piece fasse le traitement sur la piece.
		//mais si le temps dépasse les 20 secondes de traitement (ts), la MACHINE passe en défaillante
		if(pthread_cond_timedwait(&(ma->dormir),&(ma->mutMachine),&ts) != 0){
			pthread_mutex_unlock(&(ma->mutMachine));
			ma->defaillant = 1;
			printf("%d ECHEC TIMEDWAIT\n",ma->numMachine);
			break;
		}

		sleep(2); //on imagine qu'on etudie le compte rendu
		pthread_mutex_unlock(&(ma->mutMachine));

		//printf("%d SUCESS TIMEDWAIT et envoie un signal a machine\n",ma->numMachine);


		pthread_mutex_lock(&AttentRetrait); //VOIR LIGNE 246 POUR L'AUTRE CAS!! on lock le robot de retrait
			//en mettant le lock ici, on est sur que le robot de retrait retire la piece de ce robot!
			//moins rapide

		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->attendre)); //on reveille la machine pour qu'elle finisse son execusion et se remette en attente.
		pthread_mutex_unlock(&(ma->mutMachine));

		//printf("%d Attend reponse de machine\n",ma->numMachine);

		pthread_cond_wait(&(ma->dormir),&(ma->mutMachine)); //on attend que la piece soit posé sur le convoyeur.
		pthread_mutex_unlock(&(ma->mutMachine));

		//printf("%d On reveille ROBOTRETRAIT\n",ma->numMachine);

		//pthread_mutex_lock(&AttentRetrait); //VOIR LIGNE 232 POUR L'AUTRE CAS!! on lock le robot de retrait
			//en mettant le lock ici, on est pas sur que le robot de retrait retire la piece de ce robot
			//plus rapide

		pthread_mutex_lock(&mutexRetrait);
		pthread_cond_broadcast(&RobotRetrait); //on signal au robot de retrait + son suivi qu'une piece arrive en le reveillant
		pthread_mutex_unlock(&mutexRetrait);

		pthread_cond_wait(&(ma->dormir),&(ma->mutMachine)); //on attend que la piece soit extraite du conv
		pthread_mutex_unlock(&(ma->mutMachine)); //on libre le mutex
		pthread_mutex_unlock(&AttentRetrait); //on libere le mutex

		printf("%d Machine traitement fini\n",ma->numMachine);
	}
	pthread_exit(NULL);
}



void initaliserSuiviMachine()
{
  pthread_mutex_init(&MitSurRobotAlim,NULL);
  pthread_mutex_init(&AttentRetrait,NULL);
  //..............
  pthread_mutex_init(&mutexAlim,NULL);
  pthread_cond_init(&RobotAlim,NULL);
  pthread_mutex_init(&mutexRetrait,NULL);
  pthread_cond_init(&RobotRetrait,NULL);
  pthread_mutex_init(&mutexSuiviAlim,NULL);
  pthread_cond_init(&RobotSuiviAlim,NULL);
  pthread_mutex_init(&mutexSuiviRetrait,NULL);
  pthread_cond_init(&RobotSuiviRetrait,NULL);
  //..............
  pthread_t * maListeSuiviMachine=malloc(NbMachine*sizeof(machine));
  int i;
  for (i = 0; i < NbMachine; i++) {
  	pthread_create(&(maListeSuiviMachine[i]), &thread_attr, threadSuiviMachine, maListeMachine[i]);
  }
}


void * fonc_SuiviRobotAlim(void * arg) {
	int rc;
	struct timeval tp;
	struct timespec ts;
	while(EnMarche == 1){
		pthread_cond_wait(&RobotSuiviAlim,&mutexSuiviAlim);
		pthread_mutex_unlock(&mutexSuiviAlim); //on débloque le mutex
		rc = gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec*1000;
		ts.tv_sec += 20;
		//on s'endort le temps que la piece soit posé sur le convoyeur.
		//mais si le temps dépasse les 20 secondes de traitement (ts), le SYSTEME passe en défaillant
		if(pthread_cond_timedwait(&RobotSuiviAlim,&mutexSuiviAlim,&ts) != 0){
			pthread_mutex_unlock(&mutexSuiviAlim);
			printf("Robot Alim ECHEC TIMEDWAIT\n");
			EnMarche = 0;
			break;
		}
		printf("Robot Alim SUCCESS TIMEDWAIT\n");
		pthread_mutex_unlock(&mutexSuiviAlim);
	}
	pthread_exit(NULL);
}

void * fonc_SuiviRobotRetrait(void * arg) {
	int rc;
	struct timeval tp;
	struct timespec ts;
	while(EnMarche == 1){
		pthread_cond_wait(&RobotSuiviRetrait,&mutexSuiviRetrait);
		pthread_mutex_unlock(&mutexSuiviRetrait);
		rc = gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec*1000;
		ts.tv_sec += 30;
		//on s'endort le temps que la piece soit posé sur le convoyeur.
		//mais si le temps dépasse les 20 secondes de traitement (ts), le SYSTEME passe en défaillant
		if(pthread_cond_timedwait(&RobotSuiviRetrait,&mutexSuiviRetrait,&ts) != 0){
			pthread_mutex_unlock(&mutexSuiviRetrait);
			printf("Robot Retrait ECHEC TIMEDWAIT\n");
			EnMarche = 0;
			break;
		}
		printf("Robot Retrait SUCCESS TIMEDWAIT\n");
		pthread_mutex_unlock(&mutexSuiviRetrait);
	}
	pthread_exit(NULL);
}

void Superviseur()
{
	EnMarche = 1; //le systeme fonctionne
	initaliserSuiviMachine();
	pthread_create(&thread_SuiviRobotAlim, &thread_attr, fonc_SuiviRobotAlim, NULL);
	pthread_create(&thread_SuiviRobotRetrait, &thread_attr, fonc_SuiviRobotRetrait, NULL);
}
