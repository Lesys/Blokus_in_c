
#ifndef GESTION_PARTIE_SDL_H
#define GESTION_PARTIE_SDL_H

#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/affichage.h"
#include "../include/gestion_tour.h"



int initialisation_partie_sdl(Joueur** j); /*Initialisation de la partie, appel des fonctions pour crée les joueurs, le plateau*/

int fin_de_partie_sdl(Joueur** j); /* Affiche les résultats, propose les options de fin de partie et renvoie le résultat correspondant */

int jouer_tour_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j);

int jouer_manche_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j);

int jouer_partie_sdl();

#endif
