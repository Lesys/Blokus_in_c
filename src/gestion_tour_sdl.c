/**
 *	\file gestion_tour_sdl.c
 *	\brief Fichier de fonctions permettant de gérer le tour d'un joueur
 *  \details Contient les fonctions demandant la pièce à jouer, son orientation et ses coordonnées au joueur
     ainsi que les fonctions de vérification de position et de couleur et de pose de celle-ci
 *  \author RIGUIDEL Hugo
 *	\version 1.0
 *	\date 18/03/2019
 */

#include "../include/gestion_tour_sdl.h"
#include "../include/affichage.h"
#include "../include/carre.h"
#include "../include/affichage_sdl.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

extern SDL_Renderer* renderer;

void changer_orientation(Piece* p)
{
    Carre* c = piece_liste_carre(p);

    piece_pivoter(3, c);
}

int verification_position_sdl(Couleur pl[20][20], int x, int y, Piece* p)
{
    Carre* c = piece_liste_carre(p);

    do
    {
        if(pl[x+carre_get_x(c)][y+carre_get_y(c)] != VIDE || x+carre_get_x(c) >= TAILLE_PLATEAU || y+carre_get_y(c) >= TAILLE_PLATEAU)
        {
            return 0;
        }
        c = carre_get_suiv(c);
    } while(c != piece_liste_carre(p));
    return 1;
}

static int coord_dans_plateau(int coord)
{
    return (coord >= 0 && coord < TAILLE_PLATEAU);
}

/* Fonction qui vérifie si aucun Carre de la Couleur du Joueur n'est adjacant aux Carre que le Joueur veut poser
	et qu'il y a au moins un Carre de la Couleur du Joueur qui est en diagonale d'un Carre que le Joueur veut poser */
static int verification_couleur_sdl(Couleur pl[20][20], int x, int y, Couleur col, Piece* p)
{
    Carre* c = piece_liste_carre(p);
    int angle = 0;

    do
    {
	/* Vérifie qu'il n'y a aucun Carre adjacant aux Carre que le Joueur pose */
        if((coord_dans_plateau(x + carre_get_x(c) - 1) && coord_dans_plateau(y + carre_get_y(c)) && pl[x + carre_get_x(c) - 1][y + carre_get_y(c)] == col) || /* Au dessus */
		(coord_dans_plateau(x + carre_get_x(c) + 1) && coord_dans_plateau(y + carre_get_y(c)) && pl[x + carre_get_x(c) + 1][y + carre_get_y(c)] == col) || /* En dessous */
		(coord_dans_plateau(x + carre_get_x(c)) && coord_dans_plateau(y + carre_get_y(c) - 1) && pl[x + carre_get_x(c)][y + carre_get_y(c) - 1] == col) || /* A gauche */
		(coord_dans_plateau(x + carre_get_x(c)) && coord_dans_plateau(y + carre_get_y(c) + 1) && pl[x + carre_get_x(c)][y + carre_get_y(c) + 1] == col)) /* A droite */
        {
            return 0;
        }

	/* Vérifie qu'il y a au moins un Carre que le Joueur pose qui touche diagonalement un Carre déjà posé de même Couleur */
        if((coord_dans_plateau(x + carre_get_x(c) - 1) && coord_dans_plateau(y + carre_get_y(c) - 1) && pl[x + carre_get_x(c) - 1][y + carre_get_y(c) - 1] == col) || /* Diagonale Haut - Gauche */
		(coord_dans_plateau(x + carre_get_x(c) + 1) && coord_dans_plateau(y + carre_get_y(c) - 1) && pl[x + carre_get_x(c) + 1][y + carre_get_y(c) - 1] == col) || /* Diagonale Bas - Gauche */
		(coord_dans_plateau(x + carre_get_x(c) - 1) && coord_dans_plateau(y + carre_get_y(c) + 1) && pl[x + carre_get_x(c) - 1][y + carre_get_y(c) + 1] == col) || /* Diagonale Haut - Droit */
		(coord_dans_plateau(x + carre_get_x(c) + 1) && coord_dans_plateau(y + carre_get_y(c) + 1) && pl[x + carre_get_x(c) + 1][y + carre_get_y(c) + 1] == col)) /* Diagonale Bas - Droit */
        {
            angle = 1;
        }

        c = carre_get_suiv(c);

    } while(c != piece_liste_carre(p));

    /* Si au moins un Carre est en diagonale d'un Carre de même Couleur déjà posé */
    if(angle)
    {
        return 1;
    }

    /* Si aucun return n'a été fait précédemment */
    return 0;
}

int verifier_coordonnees(Couleur pl[20][20], Piece* pi, int x, int y, Joueur* j)
{
    int x_depart;
    int y_depart;
    Carre* c;

    switch(joueur_couleur(j))
    {
        case BLEU:
            x_depart = BLUE_X;
            y_depart = BLUE_Y;
            break;

        case JAUNE:
            x_depart = YELLOW_X;
            y_depart = YELLOW_Y;
            break;

        case ROUGE:
            x_depart = RED_X;
            y_depart = RED_Y;
            break;

        case VERT:
            x_depart = GREEN_X;
            y_depart = GREEN_Y;
            break;

        default:
            break;
    }

    /* Si la Piece que le Joueur pose est sa toute première Piece, doit jouer dans son coin */
    if(joueur_nb_piece_restantes(j) == NB_PIECES)
    {
        int coin = 0;
        int valide = 1;
        Carre* c2;

        c = piece_liste_carre(pi);

        c2 = c;

        do
        {
            if((y + carre_get_y(c) == y_depart) && (x + carre_get_x(c) == x_depart))
            {
                coin = 1;
            }

            c = carre_get_suiv(c);

        } while(coin == 0 && c != c2);

        if(!coin || !verification_position_sdl(pl, x, y, pi))
        {
            return 0;
        }
        return 1;
    }
    else
    {
        if(!verification_position_sdl(pl, x, y, pi) || !verification_couleur_sdl(pl, x, y, joueur_couleur(j), pi))
        {
            return 0;
        }
    }
    return 1;
}

void poser_piece_sdl(Couleur pl[20][20], Piece* pi, Joueur* j, int x, int y)
{
    if(!piece_hors_liste(pi))
    {
        Carre* c = piece_liste_carre(pi);
        Piece** p = &(j->liste_piece);
        Piece* pivot = *p;

        do
        {
            pl[x+carre_get_x(c)][y+carre_get_y(c)] = joueur_couleur(j);
            c = carre_get_suiv(c);
        } while(c != piece_liste_carre(pi));

        while ((*p) != pi)
        {
            *p = piece_suivant(*p);
        }

    	if (pivot == *p)
    		pivot = NULL;

        if(joueur_nb_piece_restantes(j) == 1 && c == carre_get_suiv(c))
            j->score += 5;

        liste_piece_suppr_elem(p);

        while (pivot != NULL && (*p) != pivot)
            *p = piece_suivant(*p);

        j->liste_piece = *p;
    }
}

int selection_piece(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j, Reserves* r, Piece** p, Bouton* b_abandonner, Bouton* b_sauvegarder)
{
    int etat = 0;

    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            etat = 2;
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            *p = curs_hover_piece(r, joueur_couleur(j));
            if(curs_hover_bouton(b_abandonner))
            {
                etat = 1;
            }
		else if( curs_hover_bouton(b_sauvegarder)){
			etat = 3;
		}

        }
        else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && *p)
        {
            int x, y;
            if(curs_hover_plateau(pl, &x, &y))
            {
                if(verifier_coordonnees(pl, *p, x, y, j))
                {
                    poser_piece_sdl(pl, *p, j, x, y);

                    /* Récupère l'id de la pièce en négatif (pour jouer_manche_sdl) */
                    etat = piece_id(*p) * -1;
                    *p = NULL;
                }
                else
                {
                    *p = NULL;
                }
            }
            else
            {
                *p = NULL;
            }
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT && *p != NULL)
        {
            changer_orientation(*p);
        }
    }

    return etat;
}

static void colorer_selection(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Piece* p, int x, int y)
{
    int obstruction = 0;

    Carre* c = piece_liste_carre(p);
    Carre* c2 = c;

    do
    {
        if(coord_dans_plateau(x + carre_get_x(c2)) && coord_dans_plateau(y + carre_get_y(c2)) && pl[x + carre_get_x(c2)][y + carre_get_y(c2)] != VIDE)
            obstruction = 1;

        c2 = carre_get_suiv(c2);

    } while(c != c2);

    do
    {
        if(coord_dans_plateau(x + carre_get_x(c2)) && coord_dans_plateau(y + carre_get_y(c2)) && pl[x + carre_get_x(c2)][y + carre_get_y(c2)] == VIDE)
            if(obstruction)
                pl[x + carre_get_x(c2)][y + carre_get_y(c2)] = SELECTION_ERREUR;
            else
                pl[x + carre_get_x(c2)][y + carre_get_y(c2)] = SELECTION;

        c2 = carre_get_suiv(c2);

    } while(c != c2);
}

int gestion_tour_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j)
{
    int etat = 0;

    Piece* p = NULL;

    Reserves* r = init_afficher_pieces_dispo_sdl(j);

    Bouton* b_abandonner = init_bouton_sdl(ABANDONNER);
    Bouton* b_sauvegarder = init_bouton_sdl(SAUVEGARDER);

    while(!etat)
    {
        SDL_RenderClear(renderer);

        etat = selection_piece(pl, j, r, &p, b_abandonner, b_sauvegarder);

        int x, y;

        if(curs_hover_plateau(pl, &x, &y) && p != NULL)
        {
            Couleur pl2[TAILLE_PLATEAU][TAILLE_PLATEAU];

        	int k, l;

        	/* Recopie du plateau */
        	for (k = 0; k < TAILLE_PLATEAU; k++)
        		for (l = 0; l < TAILLE_PLATEAU; l++)
        			pl2[k][l] = pl[k][l];

            colorer_selection(pl2, p, x, y);

            afficher_plateau_sdl(pl2);
        }
        else
            afficher_plateau_sdl(pl);

        afficher_pieces_dispo_sdl(r, j, p);
        afficher_scores_sdl(j);
        afficher_tour_sdl(j);
        afficher_bouton_sdl(b_abandonner);
        afficher_bouton_sdl(b_sauvegarder);

        SDL_RenderPresent(renderer);
    }

    free_afficher_pieces_dispo_sdl(&r);
    free_bouton_sdl(&b_abandonner);
    free_bouton_sdl(&b_sauvegarder);

    return etat;
}


















/* */
