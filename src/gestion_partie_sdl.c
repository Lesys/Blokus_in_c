/**
	*\file gestion_partie_sdl.c
	*\brief Regroupent toutes les fonctions gestion_partie_sdl.c
	*\details Toutes les fonctions qui permettent de gerer une partie de blokus en respectant les règles.
	*\author JODEAU Alexandre

*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/gestion_tour_sdl.h"
#include "../include/gestion_partie_sdl.h"
#include "../include/gestion_partie.h"
#include "../include/affichage_sdl.h"

extern SDL_Renderer* renderer;

/**
	*\fn void initialisation_partie_sdl(Joueur **j)
	*\details Initialise une partie <br> Crée une liste de n Joueur [2-4].
	<br>Initialise le pseudo des joueurs
	* Si la liste existe, on la supprime puis on en crée une autre.
	*\param j Pointeur sur un Joueur pour créer la liste de Joueur.
	*\return Retourne 3 si le joueur appuis sur la croix de l'aficheur<br>
		Retourne 0 si l'affectation a bien fonctionné
*/
int initialisation_partie_sdl(Joueur** j ){ /*Initialisation de la partie, appel des fonctions pour crées les joueurs, le plateau*/
	int nb_joueur=-1;
	int continuer=1;

/********PARTIE SAISI NB DES JOUEURS*********/


	/*Creation des boutons + evenement */
	SDL_Event event;
	Bouton* b_nb_deux=init_bouton_sdl(NB_JOUEURS_2);
	Bouton* b_nb_trois=init_bouton_sdl(NB_JOUEURS_3);
	Bouton* b_nb_quatre=init_bouton_sdl(NB_JOUEURS_4);

	/*Si une liste de joueur existe déjà, on la supprime*/
	if( (*j) != NULL ){
		joueur_liste_detruire(j);
	}
	/*Tant que l'evenenement n'est pas fini*/
	while(continuer == 1){
		SDL_RenderClear(renderer);
		//Attend un événement
		while(SDL_PollEvent(&event)){
			//Si il appuis sur la croix
			if(event.type == SDL_QUIT)
				return 3;
			//Si il appuis sur un bouton
			else if(event.type == SDL_MOUSEBUTTONDOWN){
				/*Bouton 2 joueur*/
				if (curs_hover_bouton(b_nb_deux))
					nb_joueur=2;

				/*Bouton 3 joueur*/
				else if (curs_hover_bouton(b_nb_trois))
					nb_joueur=3;

				/*Bouton 4 joueur*/
				else if (curs_hover_bouton(b_nb_quatre))
					nb_joueur=4;


			}
		}
		//Si on appuis sur un bouton, alors on arrete la boucle
		if( nb_joueur > 0){
			continuer=0;
		}
		/*Partie Affichage*/
		afficher_nb_joueurs_sdl();
		afficher_bouton_sdl(b_nb_deux);
		afficher_bouton_sdl(b_nb_trois);
		afficher_bouton_sdl(b_nb_quatre);
		SDL_RenderPresent(renderer);

	}
	free_bouton_sdl(&b_nb_deux);
	free_bouton_sdl(&b_nb_trois);
	free_bouton_sdl(&b_nb_quatre);

	*j=joueur_liste_creation(nb_joueur);

/********PARTIE SAISI NOM DES JOUEURS*********/
	SDL_Event event_saisie;
	continuer=1;
	Joueur* j_pivot = *j;
	/*While tous les joueurs n'ont pas de pseudo*/
	do {
		continuer=1;
		SDL_StartTextInput();
		/*Boucle d'évenement*/
		while(continuer){

			SDL_RenderClear(renderer);
			/*Attend l'appuis d'une touche*/
			while(SDL_PollEvent(&event_saisie)){

				/*Si c'est la croix, on arrete*/
				if(event_saisie.type == SDL_QUIT)
					return 3;

				/*Si c'est la touche entrée, on passe au joueur suivant*/
				else if(strlen((*j)->pseudo) > 0 && event_saisie.type == SDL_KEYDOWN && (event_saisie.key.keysym.sym == SDLK_RETURN || event_saisie.key.keysym.sym == SDLK_KP_ENTER) )
					continuer = 0;

				/*Si c'est une touche supprimer, on efface le dernier caractère saisie*/
				else if(event_saisie.key.keysym.sym == SDLK_BACKSPACE && event_saisie.type == SDL_KEYDOWN){
					if (strlen((*j)->pseudo) > 0)
						(*j)->pseudo[strlen((*j)->pseudo) - 1] = '\0';
				}

				/*Si c'est une touche du clavier, on l'entre dans le pseudo*/
				else if(event_saisie.type == SDL_TEXTINPUT && strlen((*j)->pseudo) < TAILLE_PSEUDO) {
					strcat((*j)->pseudo, event_saisie.text.text);
				}
			}

			afficher_saisie_pseudo_sdl(*j);
			SDL_RenderPresent(renderer);
		}
		SDL_StopTextInput();

		/* Si le pseudo n'est pas trop grand */
		if (strlen((*j)->pseudo) < TAILLE_PSEUDO) {
		        /* Réalloue la bonne taille pour le pseudo */
		        (*j)->pseudo = realloc((*j)->pseudo, sizeof(char) * (strlen((*j)->pseudo) + 1));

			(*j)->pseudo[strlen((*j)->pseudo)]='\0';
		}
		else /* S'il est trop grand: troncature */
			(*j)->pseudo[TAILLE_PSEUDO]='\0';

		*j=joueur_suivant(*j);
	} while (*j != j_pivot);
	return 0;
}




/**
	*\fn int fin_de_partie_sdl(Joueur** j)
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
		*2 - Retourne au titre. <br>

*/


/* Affiche les résultats,mets à jour le score ,propose les options de fin de partie et renvoie le résultat correspondant */

int fin_de_partie_sdl(Joueur** j){
	/*Si le joueur n'a plus de piece dans sa liste, fait abandonner le joueur automatiquement*/
	if(joueur_liste_piece(*j) == NULL)
		joueur_abandonne(*j);

	/* S'il reste un joueur n'ayant pas abandonné */
	if(!(joueur_abandon(*j)))
		return 0;

	int choix=0;
	int continuer=1;

	/*Creation des boutons + evenement */
        SDL_Event event_fin;
        Bouton* b_continuer=init_bouton_sdl(CONTINUER);
        Bouton* b_quitter=init_bouton_sdl(QUITTER_PARTIE);




	/*Mise a jour du score vue que c'est la fin de la partie*/
	maj_scores(j);
	afficher_scores_sdl(*j);


	/*On demande a l'utilisateur les choix de fin de partie */
	while(continuer){
		SDL_RenderClear(renderer);
		/*On attend la touche du joueur*/
        	while(SDL_PollEvent(&event_fin)){
			/*En attendant qu'il appuis sur le bouton*/
                	if(event_fin.type == SDL_MOUSEBUTTONDOWN){
                        	if (curs_hover_bouton(b_continuer))
                                	choix= 1;

                        	else if (curs_hover_bouton(b_quitter))
                                	choix= 2;

			        }
				if( choix > 0){
			        	continuer=0;
			        }
		}
		afficher_resultats_sdl(*j);
		afficher_bouton_sdl(b_continuer);
		afficher_bouton_sdl(b_quitter);
	        SDL_RenderPresent(renderer);
	}

	return choix;
}





/**
	*\fn int jouer_tour_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur** j)
	*\details Réalise le fonctionnement d'un tour en appellant les fonctions de gestion_tour .
	*\param pl Plateau de jeu pour posez les Piece.
	*\param j Joueur qui joue actuellement.
	*\return 3 si la croix a été saisie <br>
	  renvoie le resultat de la fonction gestion_tour_sdl<br>
	*1 = Abandon du Joueur
	*2 = Quitte le jeu ( Appuis sur la croix)
	
*/



/*Appel toute les fonctions pour réalisé un tour*/
int jouer_tour_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j){
	int valeur_r;
	if(joueur_a_abandonne(*j)){
//		printf("\n Ce joueur à abandonne\n");
		*j=joueur_suivant(*j);

	}
	else{
		valeur_r=gestion_tour_sdl(pl,*j);

		if(valeur_r == 1){//Le joueur a abandoné
//			printf("Vous avez abandonné\n");
			joueur_abandonne(*j);
		}
		else if(valeur_r == 2){
			return 3;//Quitte le jeu
		}
		if(!(joueur_a_abandonne(*j)))
			*j=joueur_suivant(*j);

	}
	return valeur_r;
}


/**
	*\fn int jouer_manche_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j)
	*\brief Réalise le fonctionnement d'une manche.
	*\param pl Plateau de jeu .
	*\param j La liste de Joueur qui joue durant la manche.
	*\return Renvoie le choix des joueurs: <br>
		*2 - Retourne au menu. <br>
		*3 - Quitte le programme.
*/


int jouer_manche_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j){
	int choix;

	do{

		do{
			choix=jouer_tour_sdl(pl,&j);
			if(choix == 3)
				return choix;
			choix=fin_de_partie_sdl(&j);
		} while(!(choix));



		initialisation_manche(pl,&j);


	} while(choix == 1 );

	return choix;
}


/**
	*\fn int jouer_partie_sdl()
	*\brief Réalise l'appelle de fonction pour jouer une partie.
	*\details Crée une liste de joueur et un plateau de jeu.
	*\puis appelle initalisation_partie et debut_manche.
*/


int jouer_partie_sdl(){ /*Appel de toute les fonctions partie */
	Joueur * j = NULL;
	Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU];
	int retour = 2;
	SDL_Event event;
	Bouton* b_jouer = init_bouton_sdl(JOUER);
	Bouton* b_quitter_jeu = init_bouton_sdl(QUITTER_JEU);
	while (retour == 2){

		/* Menu */

		/* Ecouter les EVENT */
                SDL_RenderClear(renderer);
       	        while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)
				retour = 3;
			else if(event.type == SDL_MOUSEBUTTONDOWN){
				if(curs_hover_bouton(b_jouer))
					retour= 1;
				else if(curs_hover_bouton(b_quitter_jeu))
					retour= 3;
			}
		}
		/* Appuie du bouton JOUER */
		if (retour == 1) { /*Jouer*/

			retour = initialisation_partie_sdl(&j);
			if (retour == 3){ /* Si les Joueurs arrêtent le programme pendant la saisie des pseudos / nb_joueur */
				joueur_liste_detruire(&j);

				return retour;
			}

			initialisation_manche(pl, &j);

			retour = jouer_manche_sdl(pl,j);
			joueur_liste_detruire(&j);

			if (retour == 3) /* Si les Joueurs (à la fin de la partie) ne veulent plus refaire de parties */
				return retour;
		}
		/* else if Appuie sur le bouton REGLE // TODO*/
		else if (retour == 3) { /*Appuie sur le bouton Quitter || Appuie sur la croix*/
			return retour;
		}

		/* Affiche le menu */
		afficher_titres_sdl();
	 	afficher_bouton_sdl(b_jouer);
	        afficher_bouton_sdl(b_quitter_jeu);
		SDL_RenderPresent(renderer);
	}

	if (retour == 3) { /* Si les Joueurs arrêtent le programme pendant la saisie des pseudos / nb_joueur */
		joueur_liste_detruire(&j);
	}

	free_bouton_sdl(&b_jouer);
        free_bouton_sdl(&b_quitter_jeu);

	return retour;
}
