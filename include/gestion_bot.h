#ifndef GESTION_BOT_H
#define GESTION_BOT_H

#include "couleur.h"
#include "joueur.h"
#include "carre.h"

typedef struct coup Coup;

struct coup {
	Piece* p;
	int x;
	int y;
	int valeur_coup;
}

int gestion_tour_bot(Couleur pl[][], Joueur*);

Coup* bot_jouer_tour(Couleur pl[][], Joueur*);

int bot_jouer(Couleur pl[][], Joueur, int); /* int: la profondeur de coup (==nb_coup en prévision) */
int adversaire_jouer(Couleur pl[][], Joueur, Joueur, int); /* Premier Joueur: le bot qui doit jouer. Deuxième Joueur: L'adversaire pour lequel on projette le coup */

int eval_coup_bot(Couleur pl[][], Piece*, Joueur*);

#endif

/* Evaluer la valeur de la case (moins il y a de cases prises autour, plus elle a de valeur) */
