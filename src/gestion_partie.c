#include <stdio.h> 
#include <stdlib.h>
#include "../include/gestion_partie.h"


void initialisation_partie(Joueur* j ){ /*Initialisation de la partie, appel des fonctions pour crée les joueurs, le plateau*/
	int nb_joueur=-1;
	printf("Creation de la partie\n");
	do{
		printf("Veuillez saisir le nombre de joueur [2 a 4] \n");
		scanf("%d",&nb_joueur);
	}while(nb_joueur < NB_JOUEUR_MIN && nb_joueur > NB_JOUEUR_MAX);
	j=joueur_liste_creation(nb_joueur);
}

void initialisation_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur j){
	pl=Couleur pl1[TAILLE_PLATEAU][TAILLE_PLATEAU];
	joueur_liste_reinit(j);
}

/* Permet de mettre à jour les scores à la fin de la partie */ 
/* Si le petit carré a été posé en dernier, ajout des points sur le moment de la pose */

void maj_scores(Joueur* j){
	Joueur* pivot = j;
	joueur_suivant(j);
	while(&j != &pivot){
		if(joueur_liste_piece(j)== NULL){
			j->score=joueur_score(j)+15
	joueur_suivant(j);
	while(&j != &pivot){	
		}
		else{
			Carre ** pie=joueur_liste_piece(j)
			Carre ** piv=pie;
		while(joueur_liste_piece(j) != NULL ){
			*pie=carre_get_suiv(*pie);
			j->score=joueur_score(j)-nb_carre(*pie);
			liste_piece_suppr_elem(pie);
		}
	}
}



int joueur_abandon(Joueur* j){
	Joueur* pivot;
	pivot=j;
	joueur_suivant(j);
	while(&pivot != &j && joueur_a_abandonne(j)){
		joueur_suivant(j);

	}
	return pivot == j;
}

/* Affiche les résultats, propose les options de fin de partie et renvoie le résultat correspondant */
int fin_de_partie(Joueur* j){
	if(joueur_liste_piece(j) == NULL)return 1;
	if(!(joueur_abandon(j)))return 1;
	int choix=0;
	
	printf("Fin de la partie\n");

	do{
	printf("Veuillez choisir un choix\n");
	printf("Choix 1: Recommencez une manche\n");
	printf("Choix 2: Recommencez une partie\n");
	printf("Choix 3: Quittez le programme\n");
	scanf("%d",choix);
	}while(choix < 1 && choix > 3);
	if(choix == 3) afficher_resultats(j);	
	return choix;
}


/*Appel le prochain joueur à jouer et modifie la liste joueur */
Joueur* tour_suivant(Joueur* j){
	joueur_suivant(j);
	printf("%s : A toi jouer",joueur_pseudo(j));
}


/*Appel toute les fonctions pour réalisé une tour*/
void jouer_tour(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j){
	int x,y;
	Carre * piece = demander_piece(j);
	demander_orientation(piece);
	choisir_coordonee(pl[20][20],piece,&x,&y);
	poser_piece(pl,piece,joueur_couleur(j),x,y,o);
	afficher_plateau(pl);
	if(joueur_liste_piece(j) == NULL){
		maj_score(j,piece);
	}
	tour_suivant(j);
}


int jouer_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j){
	int choix;
	do{
		do{
			jouer_tour;
			choix=fin_de_partie(pl,j)
		}while(!(choix)){
		initialisation_manche();
	}while(choix == 1){
	return choix;
}



void jouer_partie(){ /*Appel de toute les fonctions partie */
	joueur * j;
	Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU];
	do{
		initialisation_partie(pl,j);
	}while(jouer_manche(pl,j)== 2));
	joueur_liste_detruire(&j);
	detruire_plateau(&pl);

}
