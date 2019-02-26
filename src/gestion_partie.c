#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>
#include "../include/gestion_partie.h"
#include "../include/affichage.h"

void initialisation_partie(Joueur** j ){ /*Initialisation de la partie, appel des fonctions pour crée les joueurs, le plateau*/
	int nb_joueur=-1;
	char c;
	if( (*j) != NULL ){
		joueur_liste_detruire(j);
	}
	printf("Creation de la partie\n");
	do{
		printf("Veuillez saisir le nombre de joueur [2 a 4] \n");
		scanf("%c",&c);

		/* Si l'utilisateur ne rentre pas un entier*/
		if (isdigit(c))
			nb_joueur = atoi(&c);

	} while(!isdigit(c) || nb_joueur < NB_JOUEUR_MIN || nb_joueur > NB_JOUEUR_MAX);

	*j=joueur_liste_creation(nb_joueur);
}

void initialisation_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur** j){
	int i,x;

	for(i=0;i < TAILLE_PLATEAU;i++){

		for(x=0;x < TAILLE_PLATEAU;x++){
			pl[i][x]=0;
		}
	}

	joueur_liste_reinit(*j);

	while(joueur_couleur(*j) != BLEU)
		*j=joueur_suivant(*j);


}

/* Permet de mettre à jour les scores à la fin de la partie */ 
/* Si le petit carré a été posé en dernier, ajout des points sur le moment de la pose */

void maj_scores(Joueur** j) {

    // On garde l'adresse du premier joueur pour
    // savoir quand arrêter

    Joueur * pivot = *j;
    do {

        if (joueur_liste_piece(*j) == NULL) {
            (*j)->score += 15;
        }

	 else {
            (*j)->score -= joueur_nb_piece_restantes(*j);
        }

        *j = joueur_suivant(*j);

    } while ((*j) != pivot);
}

/*Vérifie si tous les joueur ont abandonné */
int joueur_abandon(Joueur* j){
	Joueur* pivot;
	pivot=j;
	j=joueur_suivant(j);

	while(pivot != j && joueur_a_abandonne(j)){
		j=joueur_suivant(j);

	}

	return (pivot == j && joueur_a_abandonne(j));
}

/* Affiche les résultats, propose les options de fin de partie et renvoie le résultat correspondant */

int fin_de_partie(Joueur** j){
	/*Si le joueur n'a plus de piece dans sa liste, abandonne le joueur automatiquement*/
	if(joueur_liste_piece(*j) == NULL)
		joueur_abandonne(*j);

	/*Si tous les joueurs n'ont pas abandonnés*/
	if(!(joueur_abandon(*j)))
		return 0;

	int choix=0;
	char c;

	printf("\n Fin de la partie !!! \n");
	/*On demande a l'utilisateur les choix de fin de partie */

	do{
		printf("\nVeuillez choisir un choix\n");
		printf("1: Recommencez une manche\n");
		printf("2: Recommencez une partie\n");
		printf("3: Quittez le programme\n");
		scanf("%c",&c);

		/*Si l'utilisateur ne rentre pas un entier*/
		if(isdigit(c)){
			choix=atoi(&c);
		}


	} while(!isdigit(c) || choix < 1 || choix > 3);

	if(choix == 3) 
		afficher_resultats(*j);	
	return choix;
}


/*Appel le prochain joueur à jouer et modifie la liste joueur */

Joueur* tour_suivant(Joueur* j){
	j=joueur_suivant(j);
	char phrase[50];
	sprintf(phrase, "\n%s : A toi de jouer\n", joueur_pseudo(j));
	afficher_str_couleur(joueur_couleur(j), phrase);
	return j;
}


/*Appel toute les fonctions pour réalisé un tour*/
void jouer_tour(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j){
	int x,y;
	if(joueur_a_abandonne(*j)){
		printf("Ce joueur à abandonne\n");
		*j=tour_suivant(*j);
	}
	else{
		/*Carre * piece = demander_piece(j);
		demander_orientation(piece);
		choisir_coordonee(pl[20][20],piece,&x,&y);
		poser_piece(pl,piece,joueur_couleur(j),x,y,o);
		afficher_plateau(pl);
		*/

		carre_afficher(piece_liste_carre(joueur_liste_piece(*j)));

		printf("Saisir si vous posez une piece [1] ou non [0] pour abandonnez\n");
		scanf("%d",&x);
		if(!x){
			printf("Vous avez abandonne\n");
			joueur_abandonne(*j);
		}

		liste_piece_suppr_elem(&((*j)->liste_piece));
		if(joueur_liste_piece(*j) == NULL){
			maj_scores(j);
		}


		*j=tour_suivant(*j);
	}
}


int jouer_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j){
	int choix;

	do{
			char phrase[50];
			sprintf(phrase, "\n%s : A toi de jouer\n", joueur_pseudo(j));
			afficher_str_couleur(joueur_couleur(j), phrase);

		do{
			jouer_tour(pl,&j);
			choix=fin_de_partie(&j);
		} while(!(choix));



		initialisation_manche(pl,&j);


	} while(choix == 1);

	return choix;
}



void jouer_partie(){ /*Appel de toute les fonctions partie */
	Joueur * j = NULL;
	Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU];
	do{

		initialisation_partie(&j);

	} while(jouer_manche(pl,j)== 2);
	joueur_liste_detruire(&j);

}
