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

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

void selection_piece(Joueur* j)
{
    SDL_Event event;

    Reserves* r = init_afficher_pieces_dispo_sdl(j);
    Piece* p;

    if(SDL_PollEvent(&event))
    {
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            /*p = curs_hover_piece();*/
            printf("salut");
        }
    }

    afficher_pieces_dispo_sdl(r, j, p);
}

void gestion_jeu(Joueur* j)
{
    while(1)
    {
        selection_piece(j);
    }
}


















/* */
