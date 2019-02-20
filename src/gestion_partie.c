#include <stdio.h> 
#include <stdlib.h>
#include "../include/gestion_partie.h"

void initialisation_partie(joueur * j1){ /*Initialisation de la partie, appelle des fonctions pour crée les joueurs, le plateau*/
	int nb_joueur=-1;
	printf("Creation de la partie\n");
	do{
		printf("Veuillez saisir le nombre de joueur [2 a 4] \n");
		scanf("%d",&nb_joueur);
	}while(nb_joueur < 2 && nb_joueur > 4);
	j1=joueur_liste_creation(nb_joueur);
	creation_plateau();
}

/* Permet de mettre à jour les scores à la fin de la partie */ 
/* Si le petit carré a été posé en dernier, ajout des points sur le moment de la pose */

void maj_scores(Joueur* j,Carre *piece){
	if(joueur_liste_piece(j) == NULL){

		if(*piece == "petit_carre"){
		j->score=joueur_score(j)+5;
		}
	j->score=joueur_score(j)+15;	
	}
	else{
		Carre ** piece=joueur_liste_piece(j)
		Carre ** pivot=piece;
		while(joueur_liste_piece(j) != NULL ){
			*piece=carre_get_suiv(*piece);
			while(&pivot != &piece){
				j->score=joueur_score(j)-1;
				*piece=carre_get_suiv(*piece);
			}
			liste_piece_suppr_elem(piece);
		}
	}
}


/*Apelle toute les fonctions pour réalisé une manche*/
void jouer_manche(Couleur pl[20][20], Joueur* j){
	int x,y;
	Carre * piece = demander_piece(j);
	demander_orientation(piece);
	choisir_coordonee(pl[20][20],piece,&x,&y);
	poser_piece(pl[20][20],piece,joueur_couleur(j),x,y,o);
	afficher_plateau(pl);
	if(joueur_liste_piece(j) == NULL){
		maj_score(j,piece);
	}
	tour_suivant(j);
}

/*Appelle le prochain joueur à jouer et modifie la liste joueur */
Joueur* tour_suivant(Joueur* j){
	joueur_suivant(j);
	printf("%s : A toi de jouer",joueur_pseudo(j));
}




/* Affiche les résultats, propose les options de fin de partie et renvoie le résultat correspondant */
int fin_de_partie(Joueur* j){
	if(joueur_liste_piece(j) == NULL)return 1;
	int choix=0;
	printf("Fin de la partie\n");
	afficher_resultats(j);
	/*Affiche le pseudo gagnant ?*/
	do{
	printf("Veuillez choisir un choix\n");
	printf("Choix 1: Recommencez une manche\n");
	printf("Choix 2: Recommencez une partie\n");
	printf("Choix 3: Finir la partie\n");
	}while(choix < 1 && choix > 3);
	switch(choix){
		case 1:initialisation_manche();break;
		case 2:initialisation_partie(j);break;
		case 3:return 0;
	}
	return 1;
}


void jouer_partie(){ /*Apelle de toute les fonctions parti */
	joueur * j
	initialisation_partie(j);
	while(fin_de_partie_(Joueur* j)){
		jouer_manche(pl,j);
	}
}
