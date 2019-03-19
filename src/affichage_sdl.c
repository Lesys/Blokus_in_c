/**
 * \file affichage_sdl.c
 * \brief Fonctions d'affichages en SDL
 * \details Diverses fonctions d'affichage en interface
 * graphique pour le jeu du blokus
 * \author BASTIDE Robin
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/sprite.h"
#include "../include/affichage_sdl.h"
#include "../include/couleur.h"
#include "../include/commun.h"
#include "../include/carre.h"
#include "../include/joueur.h"

// Variables globales externes
extern SDL_Renderer * renderer;

// Variables globales de affichage_sdl
Ressources * ressources;
int taille_carre;
int largeur_ecran;
int hauteur_ecran;

/**
 * \fn static Sprite * get_sprite(Couleur couleur)
 * \brief Récupère le sprite de carré d'une couleur
 * \param couleur Couleur du sprite à récupérer
 */
static
Sprite * get_sprite(Couleur couleur) {

	switch (couleur) {
		case ROUGE:
			return ressources->carre_rouge;
			break;
		case JAUNE:
			return ressources->carre_jaune;
			break;
		case BLEU:
			return ressources->carre_bleu;
			break;
		case VERT:
			return ressources->carre_vert;
			break;
		default:
			return ressources->carre_vide;
	}
}

/**
 * \fn int init_affichage_sdl()
 * \brief Initialise le module affichage_sdl
 * \details Fonction à appeler avant tout appel a des
 * fonctions de affichage_sdl.c
 * \return 1 si tout est ok, 0 sinon
 */
int init_affichage_sdl() {

	/* Récupération taille écran dans le cas d'un plein écran
	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm)) {
		printf("Impossible d'obtenir les dimensions de l'écran : %s", SDL_GetError());
		return 0;
	}

	largeur_ecran = dm.w;
	hauteur_ecran = dm.h;
	*/

	largeur_ecran = L_FENETRE;
	hauteur_ecran = H_FENETRE;

	// Définition de la taille d'un carré
	// Cette taille sera utilisé comme "unité de mesure"
	// dans la suite de ce fichier
	taille_carre = hauteur_ecran/40;

	// Initialisation du struc ressources
	ressources = malloc(sizeof(Ressources));
	if (!ressources) {
		printf("Malloc echoué pour les ressources\n");
		return 0;
	}

	ressources->carre_vide = init_sprite("ressources/carre_vide.png", taille_carre, taille_carre);
	ressources->carre_rouge = init_sprite("ressources/carre_rouge.png", taille_carre, taille_carre);
	ressources->carre_jaune = init_sprite("ressources/carre_jaune.png", taille_carre, taille_carre);
	ressources->carre_bleu = init_sprite("ressources/carre_bleu.png", taille_carre, taille_carre);
	ressources->carre_vert = init_sprite("ressources/carre_vert.png", taille_carre, taille_carre);
	ressources->police_m = TTF_OpenFont("ressources/police.ttf", taille_carre*1.5);
	ressources->police_p = TTF_OpenFont("ressources/police.ttf", taille_carre);
	ressources->fond_score = init_sprite("ressources/fond_score.png", taille_carre*8, taille_carre*16);
	ressources->fond_tour = init_sprite("ressources/fond_tour.png", taille_carre*8, taille_carre*16);
	ressources->bouton = init_sprite("ressources/bouton.png", taille_carre*4, taille_carre*12);
	ressources->bouton_hover = init_sprite("ressources/bouton_hover.png", taille_carre*4, taille_carre*12);
	ressources->bouton_petit = init_sprite("ressources/bouton.png", taille_carre*2, taille_carre*6);
	ressources->bouton_petit_hover = init_sprite("ressources/bouton_hover.png", taille_carre*2, taille_carre*6);
	ressources->fond_resultats = init_sprite("ressources/fond_resultats.png", taille_carre*20, taille_carre*20);
	ressources->fond_titres = init_sprite("ressources/fond_titres.png", hauteur_ecran, taille_carre*96);
	ressources->fond_config = init_sprite("ressources/fond_config.png", taille_carre*16, taille_carre*43);
	ressources->fond_saisie = init_sprite("ressources/fond_saisie.png", taille_carre*2, taille_carre*16);
	ressources->tapis_rouge = init_sprite("ressources/tapis_rouge.png", taille_carre*LONG_T_BR, taille_carre*LARG_T_BR);
	ressources->tapis_bleu = init_sprite("ressources/tapis_bleu.png", taille_carre*LONG_T_BR, taille_carre*LARG_T_BR);
	ressources->tapis_jaune = init_sprite("ressources/tapis_jaune.png", taille_carre*LONG_T_VJ, taille_carre*LARG_T_VJ);
	ressources->tapis_vert = init_sprite("ressources/tapis_vert.png", taille_carre*LONG_T_VJ, taille_carre*LARG_T_VJ);
	ressources->fond_plateau = init_sprite("ressources/fond_plateau.png", taille_carre*22, taille_carre*22);

    ressources->blanc.r = 190;
	ressources->blanc.g = 190;
	ressources->blanc.b = 205;
	ressources->jaune.r = 240;
	ressources->jaune.g = 240;
	ressources->jaune.b = 0;
	ressources->vert.r = 0;
	ressources->vert.g = 180;
	ressources->vert.b = 0;
	ressources->bleu.r = 0;
	ressources->bleu.g = 0;
	ressources->bleu.b = 255;
	ressources->rouge.r = 255;
	ressources->rouge.g = 0;
	ressources->rouge.b = 0;

	return 1;
}

/**
 * \fn void free_affichage_sdl()
 * \brief Quitte le module affichage_sdl
 * \details Fonction à appeler après le dernier
 * appel aux fonctions du fichier affichage_sdl.c
 */
void free_affichage_sdl() {

	if (ressources) {
		free_sprite(&ressources->carre_vide);
		free_sprite(&ressources->carre_rouge);
		free_sprite(&ressources->carre_jaune);
		free_sprite(&ressources->carre_bleu);
		free_sprite(&ressources->carre_vert);
		TTF_CloseFont(ressources->police_m);
		TTF_CloseFont(ressources->police_p);
		free_sprite(&ressources->fond_score);
		free_sprite(&ressources->fond_tour);
		free_sprite(&ressources->bouton);
		free_sprite(&ressources->bouton_hover);
		free_sprite(&ressources->bouton_petit);
		free_sprite(&ressources->bouton_petit_hover);
		free_sprite(&ressources->fond_resultats);
		free_sprite(&ressources->fond_titres);
		free_sprite(&ressources->fond_config);
		free_sprite(&ressources->fond_saisie);
		free_sprite(&ressources->tapis_rouge);
		free_sprite(&ressources->tapis_bleu);
		free_sprite(&ressources->tapis_jaune);
		free_sprite(&ressources->tapis_vert);
		free_sprite(&ressources->fond_plateau);
		free(ressources);
	}
}

/**
 * \fn void afficher_plateau_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU])
 * \brief Affiche le plateau de jeu
 * \param pl Plateau à afficher
 */
void afficher_plateau_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {

    	afficher_sprite(ressources->fond_plateau, largeur_ecran/2 - 11*taille_carre, hauteur_ecran/2 - 11*taille_carre);
	int offset_x = largeur_ecran/2-10*taille_carre;
	int offset_y = hauteur_ecran/2-10*taille_carre;
	for (int i = 0; i < TAILLE_PLATEAU; i++) {
		for (int j = 0; j < TAILLE_PLATEAU; j++) {
			afficher_sprite(get_sprite(pl[i][TAILLE_PLATEAU - j - 1]), i * taille_carre + offset_x, j * taille_carre + offset_y);
		}
	}
}

/**
 * \fn int curs_hover_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], int *x, int *y)
 * \brief Permet de savoir si le curseur est au dessus du plateau et à quelles coordonnées
 * \param pl Plateau de jeu
 * \param x Paramètre formel pour retour coordonnée x
 * \param y Paramètre formel pour retour coordonnée y
 * \return Un pointeur sur la pièce si la souris est au dessus d'une de la bonne couleur, NULL sinon
 */
int curs_hover_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], int *x, int *y) {
	int x_mouse, y_mouse;

	// Récupération des coordonnées de la souris
	SDL_GetMouseState(&x_mouse, &y_mouse);

	int x_gauche, x_droite, y_bas, y_haut;
        x_gauche = largeur_ecran/2 - 10*taille_carre;
        x_droite = largeur_ecran/2 + 10*taille_carre;
        y_bas = hauteur_ecran/2 + 10*taille_carre;
        y_haut = hauteur_ecran/2 - 10*taille_carre;
        if (x_mouse > x_gauche && x_mouse < x_droite && y_mouse > y_haut && y_mouse < y_bas) {
            *x = (x_mouse - x_gauche) / taille_carre;
            *y = -1 * (y_mouse - y_bas) / taille_carre;
            return 1;
        }
        else {
            *x = -1;
            *y = -1;
	    return 0;
        }
}

/**
 * \fn static void afficher_piece_sdl(Carre * c, Couleur couleur, int x, int y)
 * \brief Affiche une pièce
 * \param c Liste de carré de la pièce à afficher
 * \param couleur Couleur de la pièce
 * \param x Coordonnée en x de la pièce
 * \param y Coordonnée en y de la pièce
 */
static
void afficher_piece_sdl(Carre * c, Couleur couleur, int x, int y) {

    Couleur mp[5][5] = {0};
    Carre * init = c;

	// Représentation de la pièce dans la matrice
    do {
        mp[carre_get_x(c)][carre_get_y(c)] = 1;
    } while ((c = carre_get_suiv(c)) != init);

    // Affichage
    for (int i = 0; i < 5; i++) {
    	for (int j = 0; j < 5; j++) {
    		if (mp[i][j]) {
    			afficher_sprite(get_sprite(couleur), x + taille_carre * i, y - taille_carre * (j+1));
    		}
    	}
    }
}

/**
 * \fn static int coordonnee_valide(int x, int y, int largeur, int longueur)
 * \brief Vérifie que la coordonnée est valide pour les dimensions de la matrice données
 * \param x Coordonnée en x de la pièce
 * \param y Coordonnée en y de la pièce
 * \param largeur Largeur de la matrice
 * \param longueur Longueur du tableau
 * \return 1 si valide, 0 sinon
 */
static
int coordonnee_valide(int x, int y, int largeur, int longueur) {
	return x >= 0 && y >= 0 && x < largeur && y < longueur;
}

/**
 * \fn static int placement_possible(Reserves * r, Joueur * j, Piece * l, int x, int y)
 * \brief Vérifie si le placement d'une pièce est possible dans la réserve du joueur
 * \param r Toutes les réserves de pièce
 * \param j Joueur qui possède la réserve que l'on veut tester
 * \param l pièce à tester
 * \param x Coordonnée en x de la pièce
 * \param y Coordonnée en y de la pièce
 * \return 1 si possible, 0 sinon
 */
static
int placement_possible(Reserves * r, Joueur * j, Piece * l, int x, int y) {

	Carre * c = piece_liste_carre(l);
	Carre * init = c;

	switch (joueur_couleur(j)) {
		case VERT:
			do {
				// Vérifications si une autre pièce est présente dans la
				// case, une case à coté et en diagonale
				for (int offset_x = -1; offset_x <= 1; offset_x++) {
					for (int offset_y = -1; offset_y <= 1; offset_y++) {
						if (!coordonnee_valide(x + offset_x + carre_get_x(c), y + offset_y + carre_get_y(c),
							LARG_T_VJ, LONG_T_VJ)) {
							return 0;
						}
						if (r->vert[x + offset_x + carre_get_x(c)][y + offset_y + carre_get_y(c)]) {
        					return 0;
       					}
					}
				}
    		} while ((c = carre_get_suiv(c)) != init);
			break;
		case JAUNE:
			do {
				for (int offset_x = -1; offset_x <= 1; offset_x++) {
					for (int offset_y = -1; offset_y <= 1; offset_y++) {
						if (!coordonnee_valide(x + offset_x + carre_get_x(c), y + offset_y + carre_get_y(c),
							LARG_T_VJ, LONG_T_VJ)) {
							return 0;
						}
						if (r->jaune[x + offset_x + carre_get_x(c)][y + offset_y + carre_get_y(c)]) {
        					return 0;
       					}
					}
				}
    		} while ((c = carre_get_suiv(c)) != init);
			break;
		case BLEU:
			do {
				for (int offset_x = -1; offset_x <= 1; offset_x++) {
					for (int offset_y = -1; offset_y <= 1; offset_y++) {
						if (!coordonnee_valide(x + offset_x + carre_get_x(c), y + offset_y + carre_get_y(c),
							LARG_T_BR, LONG_T_BR)) {
							return 0;
						}
						if (r->bleu[x + offset_x + carre_get_x(c)][y + offset_y + carre_get_y(c)]) {
        					return 0;
       					}
					}
				}
    		} while ((c = carre_get_suiv(c)) != init);
			break;
		case ROUGE:
			do {
				for (int offset_x = -1; offset_x <= 1; offset_x++) {
					for (int offset_y = -1; offset_y <= 1; offset_y++) {
						if (!coordonnee_valide(x + offset_x + carre_get_x(c), y + offset_y + carre_get_y(c),
							LARG_T_BR, LONG_T_BR)) {
							return 0;
						}
						if (r->rouge[x + offset_x + carre_get_x(c)][y + offset_y + carre_get_y(c)]) {
        					return 0;
       					}
					}
				}
    		} while ((c = carre_get_suiv(c)) != init);
			break;
		default:
			break;
	}

    return 1;
}

/**
 * \fn static void affecter_piece(Reserves * r, Joueur * j, Piece * l, int x, int y)
 * \brief Place une pièce dans la réserve du joueur
 * \param r Toutes les réserves de pièce
 * \param j Joueur qui possède la réserve dans laquelle on veut poser la pièce
 * \param l pièce à poser
 * \param x Coordonnée en x de la pièce
 * \param y Coordonnée en y de la pièce
 */
static
void affecter_piece(Reserves * r, Joueur * j, Piece * l, int x, int y) {

	Carre * c = piece_liste_carre(l);
	Carre * init = c;

	switch (joueur_couleur(j)) {
		case VERT:
			do {
				// On place les carrés formant la pièce un par un
       			r->vert[x + carre_get_x(c)][y + carre_get_y(c)] = l;
    		} while ((c = carre_get_suiv(c)) != init);
			break;
		case JAUNE:
			do {
       			r->jaune[x + carre_get_x(c)][y + carre_get_y(c)] = l;
    		} while ((c = carre_get_suiv(c)) != init);
			break;
		case BLEU:
			do {
       			r->bleu[x + carre_get_x(c)][y + carre_get_y(c)] = l;
    		} while ((c = carre_get_suiv(c)) != init);
			break;
		case ROUGE:
			do {
       			r->rouge[x + carre_get_x(c)][y + carre_get_y(c)] = l;
    		} while ((c = carre_get_suiv(c)) != init);
			break;
		default:
			break;
	}
}

/**
 * \fn static void disposer_pieces(Reserves * r, Joueur *  joueur)
 * \brief Dispose les pièces restantes du joueur dans sa réserve
 * de facon à prendre le moins d'espace possible à l'écran
 * \param r Toutes les réserves de pièce
 * \param j Joueur dont on veut disposer les pièces
 */
static
void disposer_pieces(Reserves * r, Joueur *  joueur) {

	Piece * l = joueur_liste_piece(joueur);
	Piece * init = l;

	// Récupération des dimensions de la réserve du joueur
	int largeur;
	int longueur;

	switch (joueur_couleur(joueur)) {
		case VERT:
		case JAUNE:
			largeur = LARG_T_VJ;
			longueur = LARG_T_VJ;
			break;
		case BLEU:
		case ROUGE:
			largeur = LARG_T_BR;
			longueur = LONG_T_BR;
		default:
			break;
	}

	// Disposition en partant du centre
	int millieu_largeur = largeur/2-1;
	int millieu_longueur = longueur/2-1;

	// Pour toutes les pièces
	do {
		// Indique si la pièce à pu être posé
		int pose = 0;
		for (int i = 0; i <= millieu_largeur && !pose; i++) {
			for (int j = 0; j <= millieu_longueur && !pose; j++) {
				// Coordonnées qui seront différentes de -1 si un emplacement est trouvé
				int x = -1;
				int y = -1;

				// Tests sur 4 carrés à la fois (emplacements symétriques par rapport au centre)
				if (placement_possible(r, joueur, l, millieu_largeur - i, millieu_longueur - j)) {
					x = millieu_largeur - i;
					y = millieu_longueur - j;
				}
				else if (placement_possible(r, joueur, l, millieu_largeur - i, millieu_longueur + j + 1)) {
					x = millieu_largeur - i;
					y = millieu_longueur + j + 1;
				}
				else if (placement_possible(r, joueur, l, millieu_largeur + i + 1, millieu_longueur + j + 1)) {
					x = millieu_largeur + i + 1;
					y = millieu_longueur + j + 1;
				}
				else if (placement_possible(r, joueur, l, millieu_largeur + i + 1, millieu_longueur - j)) {
					x = millieu_largeur + i + 1;
					y = millieu_longueur - j;
				}

				// Si emplacement trouvé
				if (x != -1) {
					affecter_piece(r, joueur, l, x, y);
					pose = 1;
				}
			}
		}
		l = piece_suivant(l);
	} while (l != init);
}

/**
 * \fn static Couleur afficher_matrice_pieces(Reserves * r, Couleur couleur, Piece * p)
 * \brief Affiche la réserve d'un joueur avec possibilité de ne pas afficher une pièce
 * \param r Toutes les réserves de pièce
 * \param couleur Couleur du joueur dont on affiche la réserve
 * \param p Piece à ne pas afficher (NULL pour tout afficher)
 * \return La couleur du joueur si la pièce a été trouvé dans la réserve de celui-ci, -1 sinon
 */
static
Couleur afficher_matrice_pieces(Reserves * r, Couleur couleur, Piece * p) {

	// Variable pour trouver à quelle joueur
	// appartient la pièce
	Couleur couleur_p = -1;

	// Position bas/gauche de la réserve à afficher
	int offset_x;
	int offset_y;

	switch (couleur) {
		case VERT:
			// Mise en place de la position de départ
			offset_x = r->pos_vert_x;
			offset_y = r->pos_vert_y;
			// Affichage du tapis
			afficher_sprite(ressources->tapis_vert, offset_x, offset_y - (LONG_T_VJ-1)*taille_carre);
			// Parcours de la matrice
			for (int i = 0; i < LARG_T_VJ; i++) {
				for (int j = LONG_T_VJ-1; j >= 0; j--) {
					// La pièce est la pièce à ne pas afficher
					if (r->vert[i][j] == p) {
						couleur_p = VERT;
					}
					else if (r->vert[i][j]) {
						afficher_sprite(get_sprite(couleur), i * taille_carre + offset_x, -j * taille_carre + offset_y);
					}
				}
			}
			break;
		case JAUNE:
			offset_x = r->pos_jaune_x;
			offset_y = r->pos_jaune_y;
			afficher_sprite(ressources->tapis_jaune, offset_x, offset_y - (LONG_T_VJ-1)*taille_carre);
			for (int i = 0; i < LARG_T_VJ; i++) {
				for (int j = LONG_T_VJ-1; j >= 0; j--) {
					if (r->jaune[i][j] == p) {
						couleur_p = JAUNE;
					}
					else if (r->jaune[i][j] ) {
						afficher_sprite(get_sprite(couleur), i * taille_carre + offset_x, -j * taille_carre + offset_y);
					}
				}
			}
			break;
		case BLEU:
			offset_x = r->pos_bleu_x;
			offset_y = r->pos_bleu_y;
			afficher_sprite(ressources->tapis_bleu, offset_x, offset_y - (LONG_T_BR-1)*taille_carre);
			for (int i = 0; i < LARG_T_BR; i++) {
				for (int j = LONG_T_BR-1; j >= 0; j--) {
					if (r->bleu[i][j] == p) {
						couleur_p = BLEU;
					}
					else if (r->bleu[i][j]) {
						afficher_sprite(get_sprite(couleur), i * taille_carre + offset_x, -j * taille_carre + offset_y);
					}
				}
			}
			break;
		case ROUGE:
			offset_x = r->pos_rouge_x;
			offset_y = r->pos_rouge_y;
			afficher_sprite(ressources->tapis_rouge, offset_x, offset_y - (LONG_T_BR-1)*taille_carre);
			for (int i = 0; i < LARG_T_BR; i++) {
				for (int j = LONG_T_BR-1; j >= 0; j--) {
					if (r->rouge[i][j] == p) {
						couleur_p = ROUGE;
					}
					else if (r->rouge[i][j]) {
						afficher_sprite(get_sprite(couleur), i * taille_carre + offset_x, -j * taille_carre + offset_y);
					}
				}
			}
			break;
		default:
			break;
	}

	return couleur_p;
}

/**
 * \fn Reserves * init_afficher_pieces_dispo_sdl(Joueur * j)
 * \brief Initialise les réserves de pièces pour un tour
 * \details Fonction à appeller avant le début du tour pour obtenir les réserves
 * \param j Liste des joueurs
 * \return Les réserves de pièces pour ce tour
 */
Reserves * init_afficher_pieces_dispo_sdl(Joueur * j) {

	Joueur * init = j;

	// Allocation des réserves
	Reserves * r = calloc(1, sizeof(Reserves));

	// Tailles d'un carré pour calcul de positions
	// en dehors de affichage_sdl.c
	r->taille_carre = taille_carre;

	// Calcul des positions bas/gauche des réserves
	r->pos_vert_x = largeur_ecran/2 - LARG_T_VJ/2 * taille_carre;
	r->pos_vert_y = hauteur_ecran/2 - 12 * taille_carre;
	r->pos_jaune_x = largeur_ecran/2 - LARG_T_VJ/2 * taille_carre;
	r->pos_jaune_y = hauteur_ecran/2 + (LONG_T_VJ+10) * taille_carre;
	r->pos_bleu_x = largeur_ecran/2 - (LARG_T_BR+11) * taille_carre;
	r->pos_bleu_y = hauteur_ecran/2 + (LONG_T_BR/2-1) * taille_carre;
	r->pos_rouge_x = largeur_ecran/2 + 11 * taille_carre;
	r->pos_rouge_y = hauteur_ecran/2 + (LONG_T_BR/2-1) * taille_carre;

	// Disposition des pièces pour tout les joueurs
	do {
		Piece* l = joueur_liste_piece(j);
		
		switch (joueur_couleur(j)) {
			case VERT:
				disposer_pieces(r, j);
				break;
			case JAUNE:
				disposer_pieces(r, j);
				break;
			case BLEU:
				disposer_pieces(r, j);
				break;
			case ROUGE:
				disposer_pieces(r, j);
				break;
			default:
				break;
		}

		j = joueur_suivant(j);
	} while (j != init);

	return r;
}

/**
 * \fn void afficher_pieces_dispo_sdl(Reserves * r, Joueur * j, Piece * p)
 * \brief Affiche les pièces disponibles, possibilité d'afficher une pièce
 * aux coordonnées de la souris
 * \param r Réserves de pièces
 * \param j Liste des joueurs
 * \param p Pièce à afficher aux coordonnées de la souris (NULL pour
 * afficher toutes les pièces à leur place)
 */
void afficher_pieces_dispo_sdl(Reserves * r, Joueur * j, Piece * p) {
	
	Joueur * init = j;
	// Variable utilisée pour récupérer la couleur de la pièces
	// à afficher aux coordonnées de la souris
	Couleur couleur_p;

	// Pour tout les joueurs
	do {
		
		Couleur couleur_p_tmp = -1;
		switch (joueur_couleur(j)) {
			case VERT:
				couleur_p_tmp = afficher_matrice_pieces(r, VERT, p);
				break;
			case JAUNE:
				couleur_p_tmp = afficher_matrice_pieces(r, JAUNE, p);
				break;
			case BLEU:
				couleur_p_tmp = afficher_matrice_pieces(r, BLEU, p);
				break;
			case ROUGE:
				couleur_p_tmp = afficher_matrice_pieces(r, ROUGE, p);
				break;
			default:
				break;
		}

		// Si la couleur à été trouvée
		// on la garde dans couleur_p
		if (couleur_p_tmp != -1) {
			couleur_p = couleur_p_tmp;
		}

		j = joueur_suivant(j);
	} while (j != init);

	// Placement de la pièces aux coordonnées de la souris
	// si une pièce a été passée et que l'on a trouvé sa couleur
	if (p && couleur_p != -1) {
		int x, y;
		// Récupérations des coordonnées de la souris
		SDL_GetMouseState(&x, &y);
		afficher_piece_sdl(piece_liste_carre(p), couleur_p, x, y);
	}
}

/**
 * \fn void free_afficher_pieces_dispo_sdl(Reserves ** r)
 * \brief Désalloue les réserves
 * \details Fonction à appeler à la fin du tour
 * \param r Réserves de pièces
 */
void free_afficher_pieces_dispo_sdl(Reserves ** r) {
	free(*r);
	*r = NULL;
}

/**
 * \fn int curs_hover_piece(Reserves * r, Couleur couleur)
 * \brief Permet de savoir si le curseur est au desus d'une pièce de la couleur choisie
 * \param r Réserves de pièces
 * \param couleur Couleur des pièces que l'on veut tester
 * \return Un pointeur sur la pièce si la souris est au dessus d'une de la bonne couleur, NULL sinon
 */
Piece * curs_hover_piece(Reserves * r, Couleur couleur) {
	
	int x, y;
	// Récupération des coordonnées de la souris
	SDL_GetMouseState(&x, &y);

	int x_gauche, x_droite, y_bas, y_haut;
	switch (couleur) {
		case JAUNE:
			x_gauche = r->pos_jaune_x;
			x_droite = x_gauche + LARG_T_VJ*taille_carre;
			y_bas = r->pos_jaune_y;
			y_haut = y_bas - LONG_T_VJ*taille_carre;
			if (x > x_gauche && x < x_droite && y > y_haut && y < y_bas) {
				return r->jaune[(x-x_gauche)/taille_carre][-1*(y-y_bas)/taille_carre+1];
			}
			break;
		case VERT:
			x_gauche = r->pos_vert_x;
			x_droite = x_gauche + LARG_T_VJ*taille_carre;
			y_bas = r->pos_vert_y;
			y_haut = y_bas - LONG_T_VJ*taille_carre;
			if (x > x_gauche && x < x_droite && y > y_haut && y < y_bas) {
				return r->vert[(x-x_gauche)/taille_carre][-1*(y-y_bas)/taille_carre+1];
			}
			break;
		case BLEU:
			x_gauche = r->pos_bleu_x;
			x_droite = x_gauche + LARG_T_BR*taille_carre;
			y_bas = r->pos_bleu_y;
			y_haut = y_bas - LONG_T_BR*taille_carre;
			if (x > x_gauche && x < x_droite && y > y_haut && y < y_bas) {
				return r->bleu[(x-x_gauche)/taille_carre][-1*(y-y_bas)/taille_carre+1];
			}
			break;
		case ROUGE:
			x_gauche = r->pos_rouge_x;
			x_droite = x_gauche + LARG_T_BR*taille_carre;
			y_bas = r->pos_rouge_y;
			y_haut = y_bas - LONG_T_BR*taille_carre;
			if (x > x_gauche && x < x_droite && y > y_haut && y < y_bas) {
				return r->rouge[(x-x_gauche)/taille_carre][-1*(y-y_bas)/taille_carre+1];
			}
			break;
		default:
			return NULL;
			break;
	}
	return NULL;
}

/**
 * \fn static void afficher_texte(char * str, TTF_Font * police, SDL_Color couleur, int x, int y)
 * \brief Affiche du texte centré par rapport à une position donnée
 * \param str Chaine de caractères à afficher
 * \param police Police du texte à afficher
 * \param couleur Couleur du texte à afficher
 * \param x Coordonnée en x du texte à afficher
 * \param y Coordonnée en y du texte à afficher
 */
static
void afficher_texte(char * str, TTF_Font * police, SDL_Color couleur, int x, int y) {

        if (str && str[0]) {
            // Création de la texture
            SDL_Surface* texte_surface = TTF_RenderText_Blended(police, str, couleur);
            SDL_Texture* texte_texture = SDL_CreateTextureFromSurface(renderer, texte_surface);

            // Création du rectangle de destination
            SDL_Rect texte_rect;
            texte_rect.w = texte_surface->w;
            texte_rect.h = texte_surface->h;
            texte_rect.x = x - texte_surface->w/2;
            texte_rect.y = y;

            // Affichage à l'écran
            SDL_RenderCopy(renderer, texte_texture, NULL, &texte_rect);

            // Désallocation de la surface et de la texture
            SDL_FreeSurface(texte_surface);
            SDL_DestroyTexture(texte_texture);
        }
}

/**
 * \fn static SDL_Color get_color(Couleur couleur)
 * \brief Permet d'obtenir la couleur en format SDL d'une couleur au format Couleur
 * \param couleur Couleur à obtenir
 * \return La couleur au format SDL
 */
static
SDL_Color get_color(Couleur couleur) {

	switch (couleur) {
		case VERT:
			return ressources->vert;
			break;
		case JAUNE:
			return ressources->jaune;
			break;
		case BLEU:
			return ressources->bleu;
			break;
		case ROUGE:
			return ressources->rouge;
			break;
		default:
			break;
	}
}

/**
 * \fn void afficher_scores_sdl(Joueur * j)
 * \brief Affiche les scores en haut à gauche de l'écran
 * \param j Liste des joueurs
 */
void afficher_scores_sdl(Joueur * j) {

	Joueur * init = j;
	// Position en y du texte qui descend à chaque joueur
	int y = taille_carre*2;
	// Score sous forme de chaine de caractères
	char score[10];

	// Affichage de la texture de fond
	afficher_sprite(ressources->fond_score, 0, 0);
	// Affichage de "SCORES" en haut des scores
	afficher_texte("SCORES", ressources->police_m, ressources->blanc, taille_carre*8, taille_carre*0.1);

	// Pour chaque joueur
	do {
		// Affichage
		afficher_texte(joueur_pseudo(j), ressources->police_p, get_color(joueur_couleur(j)), taille_carre*6, y);
		sprintf(score, "%d", joueur_score(j));
		afficher_texte(score, ressources->police_p, get_color(joueur_couleur(j)), taille_carre*14, y);

		// Descente de y pour le prochain joueur
		y += taille_carre*1.5;

		j = joueur_suivant(j);
	} while (j != init);
}

/**
 * \fn void afficher_tour_sdl(Joueur * j)
 * \brief Affiche une indication sur qui doit jouer en haut à droite de l'écran
 * \param j Liste des joueurs
 */
void afficher_tour_sdl(Joueur * j) {

	// Affichage de la texture de fond
	afficher_sprite(ressources->fond_tour, largeur_ecran - taille_carre*16, 0);
	// Affichage du texte
	afficher_texte("Tour de :", ressources->police_m, ressources->blanc, largeur_ecran - taille_carre*8, taille_carre*1.5);
	afficher_texte(joueur_pseudo(j), ressources->police_m, get_color(joueur_couleur(j)), largeur_ecran - taille_carre*8, taille_carre*4);
}

/**
 * \fn int curs_hover_bouton(Bouton * b)
 * \brief Permet de savoir si le curseur est au desus d'un bouton
 * \param b Bouton que l'on veut tester
 * \return 1 si la souris est au dessus, 0 sinon
 */
int curs_hover_bouton(Bouton * b) {

	int x, y;
	// Récupération des coordonnées de la souris
	SDL_GetMouseState(&x, &y);
	return x > b->x_gauche && x < b->x_droite && y > b->y_haut && y < b->y_bas;
}

/**
 * \fn static void afficher_bouton(char * str, int x, int y)
 * \brief Affiche un bouton de taille standard aux coordonnées données
 * \param str texte à afficher à l'intérieur du bouton
 * \param x Coordonnée en x
 * \param y Coordonnée en y
 */
static
void afficher_bouton(char * str, int x, int y) {

	afficher_sprite(ressources->bouton, x, y);
	afficher_texte(str, ressources->police_m, ressources->blanc, x + taille_carre*6 , y + taille_carre);
}

/**
 * \fn static void afficher_bouton_petit(char * str, int x, int y)
 * \brief Affiche un petit bouton aux coordonnées données
 * \param str texte à afficher à l'intérieur du bouton
 * \param x Coordonnée en x
 * \param y Coordonnée en y
 */
static
void afficher_bouton_petit(char * str, int x, int y) {

	afficher_sprite(ressources->bouton_petit, x, y);
	afficher_texte(str, ressources->police_p, ressources->blanc, x + taille_carre*3 , y + taille_carre/2);
}


/**
 * \fn static void afficher_bouton_hover(char * str, int x, int y)
 * \brief Affiche un bouton de taille standard avec sa texture "hover" aux coordonnées données
 * \param str texte à afficher à l'intérieur du bouton
 * \param x Coordonnée en x
 * \param y Coordonnée en y
 */
static
void afficher_bouton_hover(char * str, int x, int y) {

	afficher_sprite(ressources->bouton_hover, x, y);
	afficher_texte(str, ressources->police_m, ressources->blanc, x + taille_carre*6 , y + taille_carre);
}

/**
 * \fn static void afficher_bouton_petit_hover(char * str, int x, int y)
 * \brief Affiche un petit bouton avec sa texture "hover" aux coordonnées données
 * \param str texte à afficher à l'intérieur du bouton
 * \param x Coordonnée en x
 * \param y Coordonnée en y
 */
static
void afficher_bouton_petit_hover(char * str, int x, int y) {

	afficher_sprite(ressources->bouton_petit_hover, x, y);
	afficher_texte(str, ressources->police_p, ressources->blanc, x + taille_carre*3 , y + taille_carre/2);
}

/**
 * \fn Bouton * init_bouton_sdl(Type_bouton b)
 * \brief Création d'un bouton
 * \param b Type de bouton à créer
 * \return Le bouton créé
 */
Bouton * init_bouton_sdl(Type_bouton b) {

	// Allocation de la structure
	Bouton * bouton = malloc(sizeof(Bouton));

	// Mise en place du type
	bouton->type = b;

	// Calcul des coordonnées selon le type
	switch(b) {
		case ABANDONNER:
			bouton->x_gauche = largeur_ecran - taille_carre*14;
			bouton->x_droite = largeur_ecran - taille_carre*2;
			bouton->y_haut = hauteur_ecran - taille_carre*6;
			bouton->y_bas = hauteur_ecran - taille_carre*2;
			break;
		case CONTINUER:
			bouton->x_gauche = largeur_ecran/2 - taille_carre*8;
			bouton->x_droite = largeur_ecran/2 - taille_carre*2;
			bouton->y_haut = hauteur_ecran/3*2;
			bouton->y_bas = hauteur_ecran/3*2 + taille_carre*2;
			break;
		case QUITTER_PARTIE:
			bouton->x_gauche = largeur_ecran/2 + taille_carre*2;
			bouton->x_droite = largeur_ecran/2 + taille_carre*8;
			bouton->y_haut = hauteur_ecran/3*2;
			bouton->y_bas = hauteur_ecran/3*2 + taille_carre*2;
			break;
		case JOUER:
			bouton->x_gauche = largeur_ecran/2 - taille_carre*14;
			bouton->x_droite = largeur_ecran/2 - taille_carre*2;
			bouton->y_haut = hauteur_ecran/2  + taille_carre*4;
			bouton->y_bas = hauteur_ecran/2 + taille_carre*8;
			break;
		case QUITTER_JEU:
			bouton->x_gauche = largeur_ecran/2 + taille_carre*2;
			bouton->x_droite = largeur_ecran/2 + taille_carre*14;
			bouton->y_haut = hauteur_ecran/2  + taille_carre*4;
			bouton->y_bas = hauteur_ecran/2 + taille_carre*8;
			break;
		case NB_JOUEURS_2:
		case TYPE_JOUEUR_LOCAL:
			bouton->x_gauche = largeur_ecran/2 - taille_carre*19;
			bouton->x_droite = largeur_ecran/2 - taille_carre*7;
			bouton->y_haut = hauteur_ecran/2  + taille_carre*2;
			bouton->y_bas = hauteur_ecran/2 + taille_carre*6;
			break;
		case NB_JOUEURS_3:
		case TYPE_JOUEUR_DISTANT:
			bouton->x_gauche = largeur_ecran/2 - taille_carre*6;
			bouton->x_droite = largeur_ecran/2 + taille_carre*6;
			bouton->y_haut = hauteur_ecran/2  + taille_carre*2;
			bouton->y_bas = hauteur_ecran/2 + taille_carre*6;
			break;
		case NB_JOUEURS_4:
		case TYPE_JOUEUR_BOT:
			bouton->x_gauche = largeur_ecran/2 + taille_carre*7;
			bouton->x_droite = largeur_ecran/2 + taille_carre*19;
			bouton->y_haut = hauteur_ecran/2  + taille_carre*2;
			bouton->y_bas = hauteur_ecran/2 + taille_carre*6;
			break;
		default:
			break;
	}

	return bouton;
}

/**
 * \fn void afficher_bouton_sdl(Bouton * b)
 * \brief Affichage d'un bouton
 * \param b Bouton à afficher
 */
void afficher_bouton_sdl(Bouton * b) {

	// Fonctions d'affichages
	void (*aff_b)(char *, int, int);
	void (*aff_bp)(char *, int, int);

	// Choix des fonctions d'affichages en fonction
	// de si le curseur est au dessus ou non
	if (curs_hover_bouton(b)) {
		aff_b = afficher_bouton_hover;
		aff_bp = afficher_bouton_petit_hover;
	}
	else {
		aff_b = afficher_bouton;
		aff_bp = afficher_bouton_petit;
	}

	// Affichage du bouton selon son type
	switch(b->type) {
		case ABANDONNER:
			aff_b("Abandonner", b->x_gauche, b->y_haut);
			break;
		case CONTINUER:
			aff_bp("Continuer", b->x_gauche, b->y_haut);
			break;
		case QUITTER_PARTIE:
			aff_bp("Quitter", b->x_gauche, b->y_haut);
			break;
		case JOUER:
			aff_b("Jouer", b->x_gauche, b->y_haut);
			break;
		case QUITTER_JEU:
			aff_b("Quitter", b->x_gauche, b->y_haut);
			break;
		case NB_JOUEURS_2:
			aff_b("2", b->x_gauche, b->y_haut);
			break;
		case NB_JOUEURS_3:
			aff_b("3", b->x_gauche, b->y_haut);
			break;
		case NB_JOUEURS_4:
			aff_b("4", b->x_gauche, b->y_haut);
			break;
		case TYPE_JOUEUR_LOCAL:
			aff_b("Local", b->x_gauche, b->y_haut);
			break;
		case TYPE_JOUEUR_DISTANT:
			aff_b("Distant", b->x_gauche, b->y_haut);
			break;
		case TYPE_JOUEUR_BOT:
			aff_b("Bot", b->x_gauche, b->y_haut);
			break;
		default:
			break;
	}
}

/**
 * \fn void free_bouton_sdl(Bouton ** b)
 * \brief Désallocation d'un bouton
 * \param b Bouton à désallouer
 */
void free_bouton_sdl(Bouton ** b) {

	free(*b);
	*b = NULL;
}

/**
 * \fn void afficher_resultats_sdl(Joueur * j)
 * \brief Affiche les résultats de la partie en cours au
 * millieu de l'écran
 * \param j Liste des joueurs
 */
void afficher_resultats_sdl(Joueur * j) {

	// Affichage du fond
	afficher_sprite(ressources->fond_resultats, largeur_ecran/2 - taille_carre*10, hauteur_ecran/2 - taille_carre*10);

	// Variable pour garder le premier joueur
    Joueur * pj = j;
    // Variable pour savoir si le joueur a été placé dans le classement
    int p = 0;
    // Variable pour le décalage des joueurs
    Joueur * tmp1;
    Joueur * tmp2;

    // Tri des joueurs par ordre croissant de score dans un tableau
    Joueur * classement[4] = {NULL};

    do {
        p = 0;
        for (int i = 0; i < 4 && !p; i++) {
            if (classement[i] == NULL) {
                classement[i] = j;
                p++;
            }
            else if (joueur_score(j) > joueur_score(classement[i])) {
                // Placement du joueur et décalage des autres
                tmp1 = j;
                while (i < 4) {
                    tmp2 = classement[i];
                    classement[i] = tmp1;
                    tmp1 = tmp2;
                    i++;
                }
            }
        }
        j = joueur_suivant(j);
    } while (j != pj);

    // Affichage de "RESULTATS" en haut des resultats
    afficher_texte("RESULTATS", ressources->police_m, ressources->blanc, largeur_ecran/2, hauteur_ecran/4 + taille_carre*0.5);

    // Calcul de la hauteur de départ
    int y = hauteur_ecran/4 + taille_carre*4;

    // Pour tout les joueurs dans le classement
    for (int i = 0; i < 4 && classement[i]; i++) {


        // Gestion des égalités
        int r = i; // rang réel
        while (r-1 >= 0 && joueur_score(classement[r]) == joueur_score(classement[r-1])) {
            r--;
        }
        r++; // +1 pour l'affichage

        // Affichage rang
        char rang[TAILLE_PSEUDO];
        if (r == 1) {
            sprintf(rang, "Vainqueur");
        }
        else {
            sprintf(rang, "%deme", r);
        }
        afficher_texte(rang, ressources->police_p, get_color(joueur_couleur(classement[i])), largeur_ecran/2 - 6.5*taille_carre, y);

        // Affichage pseudo
        afficher_texte(joueur_pseudo(classement[i]), ressources->police_p, get_color(joueur_couleur(classement[i])), largeur_ecran/2 + 2*taille_carre, y);

        // Affichage score
        char score[10];
        sprintf(score, "%d", joueur_score(classement[i]));
        afficher_texte(score, ressources->police_p, get_color(joueur_couleur(classement[i])), largeur_ecran/2 + 8.5*taille_carre, y);

        // Décalage vers le bas pour le prochain joueur
        y += taille_carre * 3;
    }
}

/**
 * \fn void afficher_titres_sdl()
 * \brief Affiche le fond de l'écran titre
 */
void afficher_titres_sdl() {
	afficher_sprite(ressources->fond_titres, - (ressources->fond_titres->largeur - largeur_ecran)/2, 0);
}

/**
 * \fn static void afficher_fond_config()
 * \brief Affiche le fond de l'écran de configuration
 */
static
void afficher_fond_config() {
	afficher_sprite(ressources->fond_config, largeur_ecran/2 - taille_carre*21.5, hauteur_ecran/2 - taille_carre*8);
}

/**
 * \fn void afficher_nb_joueurs_sdl()
 * \brief Affiche le fond et le texte demandant le nombre de joueurs
 */
void afficher_nb_joueurs_sdl() {

	afficher_fond_config();
	afficher_texte("Choissisez le nombre de joueurs :", ressources->police_m, ressources->blanc, largeur_ecran/2, hauteur_ecran/2 - taille_carre*4);
}

/**
 * \fn void afficher_type_joueur_sdl()
 * \brief Affiche le fond et le texte demandant le type de joueur
 */
void afficher_type_joueur_sdl() {

	afficher_fond_config();
	afficher_texte("Choissisez le type du joueur :", ressources->police_m, ressources->blanc, largeur_ecran/2, hauteur_ecran/2 - taille_carre*4);
}

/**
 * \fn void afficher_saisie_pseudo_sdl(char * str)
 * \brief Affiche le fond et le texte demandant la saisie du
 * pseudo ainsi que le pseudo en cours de saisie
 * \param str Pseudo en cours de saisie
 */
void afficher_saisie_pseudo_sdl(char * str) {

	afficher_fond_config();
	afficher_sprite(ressources->fond_saisie, largeur_ecran/2 - taille_carre*8, hauteur_ecran/2 + taille_carre*2);
	afficher_texte("Entrez le nom du joueur (Entree pour validez) :", ressources->police_m, ressources->blanc, largeur_ecran/2, hauteur_ecran/2 - taille_carre*4);
	afficher_texte(str, ressources->police_m, ressources->blanc, largeur_ecran/2, hauteur_ecran/2 + taille_carre*2);
}
