#ifndef GESTION_TOUR_H
#define GESTION_TOUR_H

#include "joueur.h"
#include "carre.h"
#include "couleur.h"

typedef enum orientation{HAUT=0, DROITE, BAS, GAUCHE} Orientation;

Piece* demander_piece(Joueur* j);

void demander_orientation(Piece* p); /* Affiche les 4 orientations possibles et l'utilisateur choisit 1, 2, 3 ou 4 selon l'orientation voulue */

void choisir_coordonnee(Couleur pl[20][20], Piece* pi, int* x, int* y, Couleur c); /* Regarde si les coordonnées saisies sont dans le plateau et sur une case non prise. Renvoie les coordonnées dans x et y */

int verification_position(Couleur pl[20][20], int x,int y);

void poser_piece(Couleur pl[20][20], Piece* pi, Couleur c, int x, int y); /* Pose la pièce sur le plateau (si possible) et supprime la pièce de la liste du joueur. Si impossible de poser la pièce (car les autres cases sont prises) ==> renvoie 0. Sinon, autre chose */

void initialiser_matrice(int matrice[5][5]);

void affecter_matrice(int matrice[5][5], Carre* c);

#endif
