/**
	*\file gestion_partie.c
	*\brief Regroupent toutes les fonctions gestion_partie.c
	*\details Toutes les fonctions qui permettent de gerer une partie de blokus en respectant les règles.
	*\author JODEAU Alexandre

*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/affichage.h"
#include "../include/gestion_tour.h"
#include "../include/gestion_partie.h"
#include "../include/affichage.h"

/**
	*\fn void initialisation_partie(Joueur **j)
	*\details Initialise une partie <br> Crée une liste de n Joueur [2-4].
	* Si la liste existe, on la supprime puis on en crée une autre.
	*\param j Pointeur sur un Joueur pour créer la liste de Joueur.
*/
void initialisation_partie(Joueur** j ){ /*Initialisation de la partie, appel des fonctions pour crées les joueurs, le plateau*/
	int nb_joueur=-1;
	char c;
	if( (*j) != NULL ){
		joueur_liste_detruire(j);
	}
	printf("Creation de la partie\n");
	do{
		printf("Veuillez saisir le nombre de joueur [2 a 4] \n");
		scanf(" %c",&c);

		/* Si l'utilisateur ne rentre pas un entier*/
		if (isdigit(c))
			nb_joueur = atoi(&c);

	} while(!isdigit(c) || nb_joueur < NB_JOUEUR_MIN || nb_joueur > NB_JOUEUR_MAX);

	*j=joueur_liste_creation(nb_joueur);
}


/**
	*\fn void initialisation_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur** j)
	*\brief Initialise une manche.
	*\details Permets de réinitialisé le plateau de jeu et une liste de piece d'un Joueur.
	*\param pl Plateau de jeu à vider.
	*\param j Pointeur sur une liste de joueur afin de reinitialiser la liste de piece de chaque Joueur.
*/

/* Permets de réinitialisé le plateau de jeu et une liste de piece d'un Joueur.*/

void initialisation_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur** j){
	int i,x;
	/* Remet les valeurs du tableau à 0*/
	for(i=0;i < TAILLE_PLATEAU;i++){

		for(x=0;x < TAILLE_PLATEAU;x++){
			pl[i][x]=0;
		}
	}

	joueur_liste_reinit(*j);
	/*Vue que le Blokus commence par le joueur bleu, on remets le pointeur sur le joueur bleu*/
	while(joueur_couleur(*j) != BLEU)
		*j=joueur_suivant(*j);


}

/**
	*\fn void maj_scores(Joueur** j)
	*\details Permets de mettre à jour les scores à la fin de la partie: <br>
	* +15 si le Joueur à poser toute ces Pieces. <br>
	* -1  pour chaque carre d'une Piece.
	*\param j Prends une liste de Joueur.
*/

/*Permets de mettre à jour les scores à la fin de la partie:*/

void maj_scores(Joueur** j) {

    // On garde l'adresse du premier joueur pour
    // savoir quand arrêter

    Joueur * pivot = *j;
    do {
	/* Si le joueur n'à plus de piece */
        if (joueur_liste_piece(*j) == NULL) {
            (*j)->score += 15;
        }
	/*Alors on enleve -1 pour chaque petit carrée */
	 else {
		Piece * p=joueur_liste_piece(*j);
		Piece * pivot=p;
		do{
			(*j)->score -= piece_nb_carre(p);
			p=piece_suivant(p);
		} while(p != pivot);

        }

        *j = joueur_suivant(*j);

    } while ((*j) != pivot);
}


/**
	*\fn int joueur_abandon(Joueur* j)
	*\brief Vérifie si tous les joueurs ont abandonné.
	*\param j Reçois la liste des joueurs et vérifie la valeur d'abandon de chaque Joueur.
	\return renvoie 1 si tous les joueurs ont abandonné, 0 sinon.

*/


/*Vérifie si tous les joueurs ont abandonné.*/

int joueur_abandon(Joueur* j){
	Joueur* pivot;
	pivot=j;
	j=joueur_suivant(j);
	/*Boucle pour savoir si tous les joueurs ont abandonne*/
	while(pivot != j && joueur_a_abandonne(j)){
		j=joueur_suivant(j);

	}

	return (pivot == j && joueur_a_abandonne(j));
}



/**
	*\fn int fin_de_partie(Joueur** j)
	*\brief Vérifie si c'est vraiment la fin de la partie,modifie les scores  et réalise les choix à faire.
	*\details Si le Joueur à une liste_vide, on le fait abandonner.
	*Une fois que tous les Joueurs ont abandoné,mets à jour le score ,affiche les résultats et demande a l'utilisateur un choix:<br>
	*-Recommencez une manche.
	<br>
	*-Recommencez une partie.
	<br>
	*-Quittez le programme.
	*\param j Liste de tous les Joueurs pour vérifier s'ils ont tous abandonné.
	\return Retourne le choix de l'utilisateur (ou 0 s'il reste un Joueur en jeu):
		*1 - Recommence une manche. <br>
		*2 - Recommence une partie. <br>
		*3 - Quitte le programme.

*/


/* Affiche les résultats,mets à jour le score ,propose les options de fin de partie et renvoie le résultat correspondant */
/*Retourne le choix de l'utilisateur (ou 0 s'il reste un Joueur en jeu):
		*1 - Recommence une manche.
		*2 - Recommence une partie.
		*3 - Quitte le programme.
*/
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

	/*Mise a jour du score vue que c'est la fin de la partie*/
	maj_scores(j);
	afficher_scores(*j);


	/*On demande a l'utilisateur les choix de fin de partie */

	do{
		printf("\nVeuillez choisir un choix\n");
		printf("1: Recommencez une manche\n");
		printf("2: Recommencez une partie\n");
		printf("3: Quittez le programme\n");
		scanf(" %c",&c);

		/*Si l'utilisateur ne rentre pas un entier*/
		if(isdigit(c)){
			choix=atoi(&c);
		}


	} while(!isdigit(c) || choix < 1 || choix > 3);

	if(choix == 3)
		afficher_resultats(*j);
	return choix;
}

/**
	*\fn Joueur* tour_suivant(Joueur* j)
	*\brief Passe au prochain Joueur.
	*\param j Joueur actuel.
	*\return Renvoie le prochain Joueur.
*/


/*Appel le prochain joueur à jouer et modifie la liste Joueur */

Joueur* tour_suivant(Joueur* j){
	j=joueur_suivant(j);
	/*Affichage */
	char phrase[50];
	sprintf(phrase, "\n%s : A toi de jouer\n", joueur_pseudo(j));
	afficher_str_couleur(joueur_couleur(j), phrase);

	return j;
}


/**
	*\fn void jouer_tour(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur** j)
	*\details Réalise le fonctionnement d'un tour en appellant les fonctions de gestion_tour .
	*\param pl Plateau de jeu pour posez les Piece.
	*\param j Joueur qui joue actuellement.
*/



/*Appel toute les fonctions pour réalisé un tour*/
void jouer_tour(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j){
	int x= -1 , y = -1, a;
	Piece* piece;
	char c;
	/*Si le joueur à abandonne, on passe au joueur suivant*/
	if(joueur_a_abandonne(*j)){
		printf("\n Ce joueur à abandonne\n");
		*j=tour_suivant(*j);
	}
	else{
		/*Tant que la piece n'est pas valide ou que le joueur abandonne*/
		do{
			if(x == -1 && y == -1){
				piece=NULL;
				/*Verification saisie clavier de l'utilisateur*/
				do {
					printf("Voulez vous posez une piece? Saisir [1] pour oui [0] pour abandonnez\n");
					scanf(" %c",&c);

					/* Si l'utilisateur ne rentre pas un entier*/
					if (isdigit(c))
						a = atoi(&c);

				} while(!isdigit(c) || a < 0 || a > 1);
				/*Si le joueur choisi le choix 0, il abandonne*/
				if(!a){
					printf("Vous avez abandonné\n");
					joueur_abandonne(*j);
				}
			}
			/*Demande la piece, l'orientation et les cordonnee */

			afficher_plateau(pl);
			if(!joueur_a_abandonne(*j)){
				piece = demander_piece(*j);

				afficher_plateau(pl);

				demander_orientation(piece,*j);
				choisir_coordonnee(pl,piece,&x,&y,*j);
			}
		} while((x < 0 || y < 0 || x > (TAILLE_PLATEAU -1) || y > (TAILLE_PLATEAU  -1)) && (!joueur_a_abandonne(*j)));
		/*Si le joueur a abandonne, on ne pose pas la piece */
		if(!(joueur_a_abandonne(*j)))
			poser_piece(pl,piece,*j,x,y);

		afficher_plateau(pl);

		*j=tour_suivant(*j);

	}
}


/**
	*\fn int jouer_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j)
	*\brief Réalise le fonctionnement d'une manche.
	*\param pl Plateau de jeu .
	*\param j La liste de Joueur qui joue durant la manche.
	*\return Renvoie le choix des joueurs: <br>
		*2 - Recommence une partie. <br>
		*3 - Quitte le programme.
*/

/*Réalise le fonctionnement d'une manche.*/

int jouer_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j){
	int choix;
	/*Tant que le choix de l'utilisateur est different de 1 : voir fin_de_partie*/
	do{
		/*Affichage du premier joueur*/
		char phrase[50];
		sprintf(phrase, "\n%s : A toi de jouer\n", joueur_pseudo(j));
		afficher_str_couleur(joueur_couleur(j), phrase);
		/*Boucle de la partie, tant que les utilisateurs ne font pas de choix de parties*/
		do{
			jouer_tour(pl,&j);
			choix=fin_de_partie(&j);
		} while(!(choix));


		/*Reinitialise une manche*/
		initialisation_manche(pl,&j);


	} while(choix == 1);

	return choix;
}


/**
	*\fn void jouer_partie()
	*\brief Réalise l'appelle de fonction pour jouer une partie.
	*\details Crée une liste de joueur et un plateau de jeu.
	*\puis appelle initalisation_partie et debut_manche.
*/


void jouer_partie(){ /*Appel de toute les fonctions partie */
	/*Creation de la liste de joueur + plateau de jeu*/
	Joueur * j = NULL;
	Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU];
	/*Initialise la partie et manche, puis joue la manche tant que les choix des joueur est different de 3*/
	do{
		initialisation_partie(&j);
		initialisation_manche(pl, &j);

	} while(jouer_manche(pl,j)== 2);
	/*Destruction de la liste de joueur du a l'allocation dynamique */
	joueur_liste_detruire(&j);

}
