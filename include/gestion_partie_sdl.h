
#ifndef GESTION_PARTIE_SDL_H
#define GESTION_PARTIE_SDL_H

#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/affichage.h"
#include "../include/gestion_tour.h"


int saisir_pseudo_joueur(Joueur** j);/*Saisi le pseudo du joueur, renvoie 0 si c'est ok, 3 si problème*/

int saisir_nb_joueur();/*Saisi le nombre de joueur, renvoie le nb de joueur, -1 si problème*/

int saisir_type_joueur(Joueur** j);/*Saisi le type du joueur ,renvoie 0 si c'est ok, 3 si pbm*/

int initialiser_joueur_distant(Joueur** j);/*initialise un joueur distant*/

int initialisation_partie_sdl(Joueur** j); /*Initialisation de la partie, appel des fonctions pour crée les joueurs, le plateau*/

int fin_de_partie_sdl(Joueur** j); /* Affiche les résultats, propose les options de fin de partie et renvoie le résultat correspondant */

int jouer_tour_bot_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j);

int jouer_tour_joueur_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j);

int jouer_manche_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j);

int jouer_partie_sdl();

#endif
