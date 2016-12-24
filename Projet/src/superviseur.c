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
  maListeMachine[ope]->listeAttente=ajouterEnFin(maListeMachine[ope]->listeAttente, *nouvellePiece);
  pthread_cond_signal(&maListeMachine[ope]->dormir); //envoie le signal a la machine pour se preparer
}

/* Suivi machine du superviseur */
void * threadSuiviMachine(void * arg) {
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
}

void initaliserSuiviMachine()
{
  pthread_t * maListeSuiviMachine=malloc(NbMachine*sizeof(machine));
  int i;
  for (i = 0; i < NbMachine; i++) {
  	pthread_create(&(maListeSuiviMachine[i]), &thread_attr, threadSuiviMachine, maListeMachine[i]);
  }
}
