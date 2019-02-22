
#include "../include/gestion_partie.h"
#include "stdio.h"
#include "stdlib.h"

int main(){
/* DEBUT DU TEST */
	printf("TEST pour gestion_partie\n");
	Joueur** j;

/* Initialisation_partie */

	printf("TEST initialisation_partie\n");

	initialisation_partie(*j);

/* Tour_suivant */
	//AVANT
	printf("j1 : %s",joueur_pseudo(*j));
	*j=tour_suivant(*j);
	//APRES
	printf("j2 : %s",joueur_pseudo(*j));


/*Jouer Partie*/

	jouer_partie();

/* Jouer_Manche */

//	jouer_manche(pl,*j);

/* Maj_score */
	/*Score pour petit_carre*/
	maj_scores(*j);


/*Fin_de_partie*/

	fin_de_partie(*j);

}


