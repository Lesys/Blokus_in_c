#ifndef GESTION_JEU_SDL_H
#define GESTION_JEU_SDL_H

#include "joueur.h"
#include "carre.h"
#include "couleur.h"
#include "affichage_sdl.h"

void demander_orientation(Piece* p, Joueur* j);

void selection_piece(Joueur* j, Reserves* r, Piece** p, int* run);

int gestion_jeu(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j);

#endif
