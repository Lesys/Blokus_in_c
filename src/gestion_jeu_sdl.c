/**
 *	\file gestion_jeu_sdl.c
 *	\brief Fichier de fonctions permettant de gérer le tour d'un joueur
 *  \details Contient les fonctions demandant la pièce à jouer, son orientation et ses coordonnées au joueur
     ainsi que les fonctions de vérification de position et de couleur et de pose de celle-ci
 *  \author RIGUIDEL Hugo
 *	\version 1.0
 *	\date 18/03/2019
 */

#include "../include/gestion_tour.h"
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
        if(pl[x+carre_get_x(c)][y+carre_get_y(c)] != VIDE)
        {
            return 0;
        }
        c = carre_get_suiv(c);
    } while(c != piece_liste_carre(p));
    return 1;
}

/* Fonction qui vérifie si aucun Carre de la Couleur du Joueur n'est adjacant aux Carre que le Joueur veut poser
	et qu'il y a au moins un Carre de la Couleur du Joueur qui est en diagonale d'un Carre que le Joueur veut poser */
int verification_couleur_sdl(Couleur pl[20][20], int x, int y, Couleur col, Piece* p)
{
    Carre* c = piece_liste_carre(p);
    int angle = 0;

    do
    {
	/* Vérifie qu'il n'y a aucun Carre adjacant aux Carre que le Joueur pose */
        if(pl[x + carre_get_x(c) - 1][y + carre_get_y(c)] == col || /* Au dessus */
		pl[x + carre_get_x(c) + 1][y + carre_get_y(c)] == col || /* En dessous */
		pl[x + carre_get_x(c)][y + carre_get_y(c) - 1] == col || /* A gauche */
		pl[x + carre_get_x(c)][y + carre_get_y(c) + 1] == col) /* A droite */
        {
            return 0;
        }

	/* Vérifie qu'il y a au moins un Carre que le Joueur pose qui touche diagonalement un Carre déjà posé de même Couleur */
        if((pl[x + carre_get_x(c) - 1][y + carre_get_y(c) - 1] == col) || /* Diagonale Haut - Gauche */
		(pl[x + carre_get_x(c) + 1][y + carre_get_y(c) - 1] == col) || /* Diagonale Bas - Gauche */
		(pl[x + carre_get_x(c) - 1][y + carre_get_y(c) + 1] == col) || /* Diagonale Haut - Droit */
		(pl[x + carre_get_x(c) + 1][y + carre_get_y(c) + 1] == col)) /* Diagonale Bas - Droit */
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
        Carre* c2;

        c = piece_liste_carre(pi);

        c2 = c;
        fprintf(stderr, "1\n");

        fprintf(stderr, "Position coin: %d %d", x_depart, y_depart);
        do
        {
            fprintf(stderr, "Coord carre: %d %d", carre_get_x(c), carre_get_y(c));
            if((y + carre_get_y(c) == y_depart) && (x + carre_get_x(c) == x_depart))
            {
                fprintf(stderr, "2\n");
                coin = 1;
            }
            c = carre_get_suiv(c);
        } while(coin == 0 && c != c2);
        fprintf(stderr, "3\n");
        if(!coin)
        {
            fprintf(stderr, "4\n");
            return 0;
        }
        fprintf(stderr, "5\n");
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

void selection_piece(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j, Reserves* r, Piece** p, Bouton* b, int* run)
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            *run = 0;
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            *p = curs_hover_piece(r, joueur_couleur(j));
            if(curs_hover_bouton(b))
            {
                *run = 0;
            }
        }
        else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            int x, y;
            fprintf(stderr, "relachement\n");
            if(curs_hover_plateau(pl, &x, &y))
            {
                fprintf(stderr, "curs hover plateau : %d %d\n", x, y);
                if(verifier_coordonnees(pl, *p, x, y, j))
                {
                    fprintf(stderr, "coord correctes\n");
                    poser_piece_sdl(pl, *p, j, x, y);
                    *p = NULL;
                }
                else
                {
                    fprintf(stderr, "coord incorrectes\n");
                    *p = NULL;
                }
            }
            else
            {
                fprintf(stderr, "curs not hover plateau\n");
                *p = NULL;
            }
            fprintf(stderr, "fin\n");
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT && *p != NULL)
        {
            changer_orientation(*p);
        }
    }
}

void gestion_jeu(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j)
{
    int run = 1;

    Piece* p = NULL;

    Reserves* r = init_afficher_pieces_dispo_sdl(j);

    Bouton* b = init_bouton_sdl(ABANDONNER);

    while(run)
    {
        SDL_RenderClear(renderer);

        selection_piece(pl, j, r, &p, b, &run);

        fprintf(stderr, "fin selec\n");
        afficher_plateau_sdl(pl);
        fprintf(stderr, "fin affichage palteau\n");
        afficher_pieces_dispo_sdl(r, j, p);
        fprintf(stderr, "fin affichage pieces\n");
        afficher_scores_sdl(j);
        fprintf(stderr, "fin score\n");
        afficher_tour_sdl(j);
        fprintf(stderr, "fin tour\n");
        afficher_bouton_sdl(b);
        fprintf(stderr, "fin affichage bouton\n");

        SDL_RenderPresent(renderer);
    }

    free_afficher_pieces_dispo_sdl(&r);
    free_bouton_sdl(&b);
}


















/* */
