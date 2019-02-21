#include <stdio.h>
#include <stdlib.h>

#include "../include/piece.h"

/* Ce fichier sert à répertorier toutes les fonctions de creation pour les pièces du jeu */

Carre* piece_petit_carre() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = c;

	return c;
}

Carre* piece_petite_barre() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = c;

	return c;
}

Carre* piece_petit_coin() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 1;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 0;
	c->suiv->suiv->y = 1;
	c->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_petite_moy_barre() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 0;
	c->suiv->suiv->y = 2;
	c->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_carre() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 0;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_petit_t() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 1;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 0;
	c->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_moy_grande_barre() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 0;
	c->suiv->suiv->y = 2;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 0;
	c->suiv->suiv->suiv->y = 3;
	c->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_petit_l() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 0;
	c->suiv->suiv->y = 2;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_petit_s() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 1;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_grand_l() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 0;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 0;
	c->suiv->suiv->y = 1;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 0;
	c->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 0;
	c->suiv->suiv->suiv->suiv->y = 3;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_grand_t() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 0;
	c->suiv->suiv->y = 2;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_equerre() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 0;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 2;
	c->suiv->suiv->y = 0;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 0;
	c->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 0;
	c->suiv->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_long_s() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 1;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->suiv->y = 3;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_grand_z() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 0;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 1;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_grande_barre() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 0;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 2;
	c->suiv->suiv->y = 0;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 3;
	c->suiv->suiv->suiv->y = 0;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 4;
	c->suiv->suiv->suiv->suiv->y = 0;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_pouce() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 0;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 0;
	c->suiv->suiv->y = 1;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->suiv->y = 0;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_escalier() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 0;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 1;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_petit_u() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 0;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->y = 0;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_autre_1() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 1;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 0;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_croix() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 1;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 1;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 1;
	c->suiv->suiv->y = 0;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->y = 2;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 2;
	c->suiv->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}

Carre* piece_autre_2() {
	Carre* c = malloc(sizeof(Carre));

	c->x = 0;
	c->y = 0;
	c->suiv = malloc(sizeof(Carre));

	c->suiv->x = 0;
	c->suiv->y = 1;
	c->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->x = 0;
	c->suiv->suiv->y = 2;
	c->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->x = 0;
	c->suiv->suiv->suiv->y = 3;
	c->suiv->suiv->suiv->suiv = malloc(sizeof(Carre));

	c->suiv->suiv->suiv->suiv->x = 1;
	c->suiv->suiv->suiv->suiv->y = 1;
	c->suiv->suiv->suiv->suiv->suiv = c;

	return c;
}
