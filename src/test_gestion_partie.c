
#include "../include/gestion_partie.h"
#include "stdio.h"
#include "stdlib.h"

int main(){
/* DEBUT DU TEST */
	printf("TEST pour gestion_partie\n");
	Joueur* j;
	j=NULL;
	printf("TEST SEGF\n");

/* Initialisation_partie 

	printf("TEST initialisation_partie\n");

	initialisation_partie(&j);
	if(j == NULL){
		printf("Probleme initialisation_partie renvoie du joueur\n");
	}
	printf("TEST %s\n",j->pseudo);
 Tour_suivant 
	printf("TEST Tour_suivant\n");
	//AVANT
	printf("j1 : %s\n",joueur_pseudo(j));
	j=tour_suivant(j);
	//APRES
	printf("\nj2 : %s\n",joueur_pseudo(j));
*/

/*Jouer Partie*/

	jouer_partie();

/* Jouer_Manche */

	//jouer_manche(pl,*j);

/* Maj_score */
	/*Score pour petit_carre*/
	maj_scores(j);


/*Fin_de_partie*/

	fin_de_partie(j);

}


