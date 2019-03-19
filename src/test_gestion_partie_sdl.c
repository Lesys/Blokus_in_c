#include "../include/gestion_partie_sdl.h"
#include "stdio.h"
#include "stdlib.h"
#include "../include/sdl.h"
#include "../include/affichage_sdl.h"

extern SDL_Renderer* renderer;

int main(){
/* DEBUT DU TEST */

	sdl_init();

	fprintf(stderr, "\nAVANT SDL_RENDERCLEAR\n");
	SDL_RenderClear(renderer);
	fprintf(stderr, "\nAPRES SDL_RENDERCLEAR\n");



	Joueur* j;
	j=NULL;

/* Initialisation_partie

	printf("TEST initialisation_partie\n");

	initialisation_partie_sdl(&j);
	if(j == NULL){
		printf("Probleme initialisation_partie renvoie du joueur\n");
	}
	else{
		printf("TEST %s\n",j->pseudo);
	}
*/
/* Tour_suivant
	printf("TEST Tour_suivant\n");
	//AVANT
	printf("j1 : %s\n",joueur_pseudo(j));
	j=tour_suivant(j);
	//APRES
	printf("\nj2 : %s\n",joueur_pseudo(j));
	j=tour_suivant(j);
	//REJOUEUR1
	printf("\nj3 : %s\n",joueur_pseudo(j));
*/


/* Maj_score
	printf("TEST DEBUT  maj_score\n");
	printf("AVANT score %d",joueur_score(j));
	maj_scores(&j);
	printf("AVANT score %d",joueur_score(j));
	printf("TEST FIN  maj_score\n");
*/

/*Fin_de_partie
	printf("TEST DEBUT FIN DE PARTIE\n");
	printf("%d valeur",fin_de_partie(j));
	printf("TEST FIN FIN DE PARTIE\n");
*/
/*Jouer Partie*/

	printf("TEST DEBUT jouer_partie()\n");
	jouer_partie_sdl();
	printf("TEST FIN JOUER_PARTIE()\n");


/* Jouer_Manche */

	//jouer_manche(pl,*j);



	sdl_close();
	return 0;
}
