#include <stdio.h> 
#include <stdlib.h>
#include "../include/gestion_partie.h"


void initialisation_partie(Joueur** j ){ /*Initialisation de la partie, appel des fonctions pour crée les joueurs, le plateau*/
	int nb_joueur=-1;
	if( (*j) != NULL ){
		joueur_liste_detruire(j);
	}
	printf("Creation de la partie\n");
	do{
		printf("Veuillez saisir le nombre de joueur [2 a 4] \n");
		scanf("%d",&nb_joueur);
	}while(nb_joueur < NB_JOUEUR_MIN || nb_joueur > NB_JOUEUR_MAX);
	*j=joueur_liste_creation(nb_joueur);
	printf("Fin d'initialisation_partie\n");
}

void initialisation_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur** j){
	int i,j;
	for(i=0;i < TAILLE_PLATEAU;i++){
		for(j=0;j < TAILLE_PLATEAU;j++){
			pl[i][j]=0;
		}
	}
	joueur_liste_reinit(*j);
}

/* Permet de mettre à jour les scores à la fin de la partie */ 
/* Si le petit carré a été posé en dernier, ajout des points sur le moment de la pose */

void maj_scores(Joueur** j) {
 
    // On garde l'adresse du premier joueur pour
    // savoir quand arrêter
    Joueur * pivot = *j;
	printf("Debut de maj_score\n");
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
	printf("TEST SEGFAULT\n");
	if(!(joueur_abandon(j)))return 1;
	int choix=0;
	
	printf("Fin de la partie\n");

	do{
	printf("Veuillez choisir un choix\n");
	printf("Choix 1: Recommencez une manche\n");
	printf("Choix 2: Recommencez une partie\n");
	printf("Choix 3: Quittez le programme\n");
	scanf("%d",&choix);
	}while(choix < 1 || choix > 3);
	if(choix == 3) printf("Affichage Resultat\n"); //afficher_resultats(j);	
	return choix;
}


/*Appel le prochain joueur à jouer et modifie la liste joueur */
Joueur* tour_suivant(Joueur* j){
	j=joueur_suivant(j);
	printf("%s : A toi jouer",joueur_pseudo(j));
	return j;
}


/*Appel toute les fonctions pour réalisé une tour*/
void jouer_tour(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j){
	int x,y;
	
	/*Carre * piece = demander_piece(j);
	demander_orientation(piece);
	choisir_coordonee(pl[20][20],piece,&x,&y);
	poser_piece(pl,piece,joueur_couleur(j),x,y,o);
	afficher_plateau(pl);
	*/
	printf("Appel gestion_tour\n");
	int i;
	for(i=0 ; i < 2; i++){
		tour_suivant(j);
	}
	j->liste_piece=NULL;
	printf("debut_majscores\n");
	if(joueur_liste_piece(j) == NULL){
		maj_scores(&j);
	}
	printf("Fin de maj score\n");

	tour_suivant(j);

}


int jouer_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j){
	int choix;
	do{
		do{
			jouer_tour(pl,j);
			printf("JOUER MANCHE DEBUT FIN_de_PARTIE\n");
			choix=fin_de_partie(j);
		}while(!(choix));
		initialisation_manche(pl,&j);
	}while(choix == 1);
	return choix;
}



void jouer_partie(){ /*Appel de toute les fonctions partie */
	Joueur * j = NULL;
	Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU];
	do{
		initialisation_partie(&j);
	}while(jouer_manche(pl,j)== 2);
	joueur_liste_detruire(&j);

}
