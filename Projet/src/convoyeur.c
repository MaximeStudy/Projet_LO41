#include "../header/convoyeur.h"
#include "../header/include.h"
#include "../header/robot.h"

void * fonc_convoyeur(void * arg) {
  int i = 0;
	pthread_mutex_lock(&mtx_menu);
	pthread_cond_signal(&Cmenu);
	pthread_mutex_unlock(&mtx_menu);
  while(1)
  {
    //sleep(vitesseConv);
    usleep(vitesseConv*400000+400000);
    pthread_mutex_lock(&mutexConvoyeur);
    tournerConvoyeur();

    afficherConvoyeur();

    if (i % 2 == 0){
      pthread_cond_broadcast(&condPose);//fonctionne comme un pthread_cond_signal mais prévient TOUS les threads en wait
      i++;
    }
    else {
      //pthread_cond_broadcast(&condPose2);
      i = 0;
    }
    pthread_mutex_unlock(&mutexConvoyeur);

  }
  pthread_exit(NULL);
}

void initialiserConvoyeur(int vit){
    num=0; //compteur pour les id pieces

    tailleConv=(NbMachine+2)*2; //deux places pour les machines + 2 pour chaque robots (alim et retrait)
    vitesseConv=vit;
    pthread_mutex_init(&mutexConvoyeur,NULL);
    conv=malloc(tailleConv*sizeof(piece));
    pieceVideConv = malloc(sizeof(piece));
    pieceVideConv->num = -1;
    pieceVideConv->ope = -1;
    int i;
    for (i =0 ; i<tailleConv; i++){
        conv[i] = *pieceVideConv;//*pieceVideConv;
    }
    pthread_create(&thread_convoyeur, &thread_attr, fonc_convoyeur, NULL);

}

void libererConvoyeur(){
	free(pieceVideConv);
	free(conv); //libère les ressources alloué
}

void tournerConvoyeur()
{
	//afficherConvoyeur();
	piece tmp=conv[tailleConv-1];
	int i;
	for(i=tailleConv-1;i>0;i--)
	{
		conv[i]=conv[i-1];
	}
	conv[0]=tmp;
}

void ajouterPieceConvoyeur(int position, piece p)
{
  if(position<tailleConv && position>=0)
  {
    conv[position]=p;
  }
}

piece retirerPieceConvoyeur(int position)
{
  if(position<tailleConv && position>=0)
  {
    piece p = conv[position];
    conv[position]=*pieceVideConv;
    return p;
  }
}

void afficherConvoyeur()
{
    /* Tant que l'on n'est pas au bout de la liste */
    printf("*****************************************\n");
    int i;
    for(i=0;i<tailleConv;i++)
    {
        /* On affiche */
        if(conv[i].num!=-1)
        {
          printf("conv[%2d] : %5d \n",i, conv[i].num);
        }
        else {
          printf("conv[%2d] : %5s \n",i,"vide");
        }
    }
}
