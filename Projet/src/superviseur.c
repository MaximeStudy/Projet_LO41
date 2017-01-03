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

}


void killThreads(void)
{
  //permet de meetre le système en defaillance TODO faire egalement les free
  if(pthread_cancel(thread_robotAlim)==0)
    printf("error thread_robotAlim\n");
  if(pthread_cancel(thread_robotRetrait)==0)
  printf("error thread_robotRetrait\n");
  if(pthread_cancel(thread_convoyeur)==0)
  printf("error thread_convoyeur\n");

  if(pthread_cancel(thread_SuiviRobotAlim)==0)
  printf("error thread_SuiviRobotAlim\n");

  if(pthread_cancel(thread_SuiviRobotRetrait)==0)
  printf("error thread_SuiviRobotRetrait\n");

  int i;
  for (i = 0; i < NbMachine; i++) {
    pthread_cancel(maListeMachine[i]->thread_id);
    pthread_cancel(maListeSuiviMachine[i]);
  }
  /* Libération des différentes ressources alouées */
  /* Liste suivi machine */
  free(maListeSuiviMachine);

  /* Machine*/
  for (i = 0; i < NbMachine; i++)
  {
    /* libere toute la liste chainée*/
    effacerListe(maListeMachine[i]->listeAttente);

    /* detruit les mutex et conditions de chaque machine */
    pthread_cond_destroy(&(maListeMachine[i]->dormir));
    pthread_cond_destroy(&(maListeMachine[i]->attendre));
    pthread_mutex_destroy(&(maListeMachine[i]->mutMachine));

    /* libère la machine */
    free(maListeMachine[i]);
  }
  free(maListeMachine);

  /* convoyeur */
  libererConvoyeur();

  /* Libération des conditions et mutex */
  /* convoyeur */
  pthread_mutex_destroy(&mutexConvoyeur);

  /* IHM*/
  //pthread_cond_destroy(&Cmenu);
  //pthread_mutex_destroy(&mtx_menu);

  /* Robot d'alimentation */
  pthread_cond_destroy(&condPose);

  /* Superviseur */
  pthread_mutex_destroy(&MitSurRobotAlim);
  pthread_mutex_destroy(&AttentRetrait);
  pthread_mutex_destroy(&mutexAlim);
  pthread_mutex_destroy(&mutexRetrait);

  pthread_cond_destroy(&RobotAlim);
  pthread_cond_destroy(&RobotRetrait);
  pthread_cond_destroy(&RobotSuiviAlim);
  pthread_cond_destroy(&RobotSuiviRetrait);

  /* remet les mode normaux*/
  modeDeg1 = 1; //désactive le mode degrade 1
  modeDeg2 = 1; //désactive le mode degrade 2
  modeDeg3 = 1; //désactive le mode degrade 3
  modeDeg4 = 1; //désactive le mode degrade 4

  num=0; //compteur pour les id pieces à 0
}

void killThreadMachine(int numMachine){
	pthread_cancel(maListeMachine[numMachine]->thread_id);
	pthread_cancel(maListeSuiviMachine[numMachine]);
}



//version 2
void * threadSuiviMachine(void * arg) {
	machine * ma=(machine *)arg;
	struct timeval tp;
	struct timespec ts;
	int rc;

	while(EnMarche == 1 && ma->defaillant == 0){

		if (recupererElementEnTete(ma->listeAttente) == NULL){ //si pas de piece pour cette machine
			pthread_cond_wait(&(ma->dormir),&(ma->mutMachine)); //on s'endort le temps d'en recevoir une
			pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex
		}
		pthread_mutex_lock(&MitSurRobotAlim); //on tente de lock pour poser la piece sur le robot d'alim
		pieceRobotAlim = recupererElementEnTete(ma->listeAttente); //on la pose
		ma->listeAttente=supprimerElementEnTete(ma->listeAttente); //on supprime la piece de la liste d'attente
		pthread_mutex_lock(&mutexAlim);
		pthread_cond_signal(&RobotAlim); //on envoie un signal au robot d'alim + son suivi pour lui dire que c'est fait
		pthread_mutex_unlock(&mutexAlim);

		pthread_cond_wait(&(ma->dormir),&(ma->mutMachine)); //on s'endort le temps que le robot fasse la manip
		pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex

		pthread_mutex_unlock(&MitSurRobotAlim); //on unlock pour que les autres thread de suivi puissent poser une piece sur le robot d'alim

		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->attendre)); //on previent la machine que la piece arrive
		pthread_mutex_unlock(&(ma->mutMachine));

		//on attend que la machine recoive la piece et lance le traitement


		rc = gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec*1000;
		ts.tv_sec += 15;
		//on s'endort le temps que la piece soit récupéré par la machine.
		//mais si le temps dépasse les 15 secondes de traitement (ts), le SYSTEME passe en défaillant
		if(pthread_cond_timedwait(&(ma->dormir),&(ma->mutMachine),&ts) != 0){
			pthread_mutex_unlock(&(ma->mutMachine));
			ma->defaillant = 1;
			EnMarche = 0;
			printf("DEFAILLANCE Machine n°%d!! \n Le systeme passe en défaillance", ma->numMachine);
      			killThreads(); //le systeme meurt
			break;
		}
		pthread_mutex_unlock(&(ma->mutMachine)); //on libere le mutex

		rc = gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec*1000;
		ts.tv_sec += 20;
		//on s'endort le temps que la piece fasse le traitement sur la piece.
		//mais si le temps dépasse les 20 secondes de traitement (ts), la MACHINE passe en défaillante
		if(pthread_cond_timedwait(&(ma->dormir),&(ma->mutMachine),&ts) != 0){
			pthread_mutex_unlock(&(ma->mutMachine));
			ma->defaillant = 1;
			printf("DEFAILLANCE Machine n°%d!! \n La machine passe en défaillante", ma->numMachine);
			killThreadMachine(ma->numMachine);
			break;
		}

		sleep(1); //on imagine qu'on etudie le compte rendu
		pthread_mutex_unlock(&(ma->mutMachine));

		pthread_mutex_lock(&AttentRetrait); //on bloque le robot de retait (donc si une autre machine veut poser une piece, elle est bloqué le temps que la piece soit retiré

		pthread_mutex_lock(&(ma->mutMachine));
		pthread_cond_signal(&(ma->attendre)); //on reveille la machine pour qu'elle finisse son execusion et se remette en attente.
		pthread_mutex_unlock(&(ma->mutMachine));

		pthread_cond_wait(&(ma->dormir),&(ma->mutMachine)); //on attend que la piece soit posé sur le convoyeur.
		pthread_mutex_unlock(&(ma->mutMachine));

		pthread_mutex_lock(&mutexRetrait);
		pthread_cond_signal(&RobotRetrait); //on signal au robot de retrait + son suivi qu'une piece arrive en le reveillant
		pthread_mutex_unlock(&mutexRetrait);

		pthread_cond_wait(&(ma->dormir),&(ma->mutMachine)); //on attend que la piece soit extraite du conv
		pthread_mutex_unlock(&(ma->mutMachine)); //on libre le mutex
		pthread_mutex_unlock(&AttentRetrait); //on libere le mutex

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
  pthread_cond_init(&RobotSuiviAlim,NULL);
  pthread_cond_init(&RobotSuiviRetrait,NULL);
  //..............
  maListeSuiviMachine=malloc(NbMachine*sizeof(machine));
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
		pthread_cond_wait(&RobotSuiviAlim,&mutexAlim);
		pthread_mutex_unlock(&mutexAlim); //on débloque le mutex
		rc = gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec*1000;
		ts.tv_sec += 20;
		//on s'endort le temps que la piece soit posé sur le convoyeur.
		//mais si le temps dépasse les 5 secondes de traitement (ts), le SYSTEME passe en défaillant
		if(pthread_cond_timedwait(&RobotSuiviAlim,&mutexAlim,&ts) != 0){
			pthread_mutex_unlock(&mutexAlim);
			printf("DEFAILLANCE ROBOT D'ALIMENTATION!! \n Le systeme passe en défaillance");
      killThreads(); //tout le systeme est KO
			EnMarche = 0;
			break;
		}
    pthread_cond_signal(&RobotAlim);

		pthread_mutex_unlock(&mutexAlim);
	}
	pthread_exit(NULL);
}

void * fonc_SuiviRobotRetrait(void * arg) {
	int rc;
	struct timeval tp;
	struct timespec ts;
	while(EnMarche == 1){
		pthread_cond_wait(&RobotSuiviRetrait,&mutexRetrait);
		pthread_mutex_unlock(&mutexRetrait);
		rc = gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec*1000;
		ts.tv_sec += 30;
		//on s'endort le temps que la piece soit posé sur le convoyeur.
		//mais si le temps dépasse les 20 secondes de traitement (ts), le SYSTEME passe en défaillant
		if(pthread_cond_timedwait(&RobotSuiviRetrait,&mutexRetrait,&ts) != 0){
			pthread_mutex_unlock(&mutexRetrait);
			EnMarche = 0;
			printf("DEFAILLANCE ROBOT DE RETRAIT!! \n Le systeme passe en défaillance");
      killThreads(); //tout le systeme est KO
			break;
		}
    pthread_cond_signal(&RobotRetrait);

		pthread_mutex_unlock(&mutexRetrait);
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
