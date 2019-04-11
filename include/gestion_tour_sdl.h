#ifndef GESTION_JEU_SDL_H
#define GESTION_JEU_SDL_H

#include "joueur.h"
#include "carre.h"
#include "couleur.h"
#include "affichage_sdl.h"

void changer_orientation(Piece* p);

int verification_position_sdl(Couleur pl[20][20], int x, int y, Piece* p);

int verifier_coordonnees(Couleur pl[20][20], Piece* pi, int x, int y, Joueur* j);

void poser_piece_sdl(Couleur pl[20][20], Piece* pi, Joueur* j, int x, int y);

int selection_piece(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j, Reserves* r, Piece** p, Bouton* b_abandonner, Bouton* b_sauvegarder);

int gestion_tour_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j);

#endif
