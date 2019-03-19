
#ifndef GESTION_PARTIE_SDL_H
#define GESTION_PARTIE_SDL_H

#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/affichage.h"
#include "../include/gestion_tour.h"



void initialisation_partie_sdl(Joueur** j); /*Initialisation de la partie, appel des fonctions pour crée les joueurs, le plateau*/

/*Initialisation de la manche*/
void initialisation_manche_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur** j);

void maj_scores_sdl(Joueur** j); /* Permet de mettre à jour les scores à la fin de la partie */ 
/* Si le petit carré a été posé en dernier , ajout des points sur le moment de la pose */

int joueur_abandon_sdl(Joueur* j);/*Si tous les joueur on abandonne*/


int fin_de_partie_sdl(Joueur** j); /* Affiche les résultats, propose les options de fin de partie et renvoie le résultat correspondant */

Joueur* tour_suivant_sdl(Joueur* j);


void jouer_tour_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j);


int jouer_manche_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j);

void jouer_partie_sdl();

#endif
