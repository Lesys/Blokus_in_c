#include <stdio.h>
#include <stdlib.h>
#include "../include/carre.h"
/* Procédures d'accès pour un carré */

/* Permet de récupérer la coordonnée x d'un Carre */
int carre_get_x (Carre* c) {
	return c->x;
}

/* Permet de récupérer la coordonnée y d'un Carre */
int carre_get_y (Carre* c) {
	return c->y;
}

/* Permet de récupérer le carré suivant d'une pièce */
Carre* carre_get_suiv (Carre* c) {
	if (!carre_hors_liste(c))
		return c->suiv;
	else
		return NULL;
}

int carre_hors_liste(Carre* c) {
	return (c == NULL);
}

void carre_reinit_tab(char tab[5][5]) {
	int i, j;

	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
			tab[i][j] = ' ';
}

/* Affiche une pièce */
void carre_afficher(Carre* c) {
	Carre* init = c;
	char tab[5][5]; /* Tableau représentant le Carre dans un plan 5/5 (pour l'affichage) */
	int i, j;

	carre_reinit_tab(tab);

	/* Tant que le Carre qu'on regarde n'est pas le premier qu'on a eu (== la boucle n'est pas terminée) */
	do {
		tab[4 - c->x][c->y] = '*'; /* Pour centrer la pièce en bas à gauche */
	} while ((c = carre_get_suiv(c)) != init)

	/* Affiche le tableau avec des ' ' (pas de Carre) ou des '*' (un Carre) */
	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
			putchar(tab[i][j]);
}


/* Libère un Carre */
void carre_free(Carre** c) {
	if (!carre_hors_liste((*c)->suiv))
		carre_get_suiv((*c)) = NULL;

	free(*c);
	(*c) = NULL;
}

/* Detruit une pièce entière */
void carre_detruire(Carre** c) {
	Carre** supp;
	*supp = *c;

	while (!carre_hors_liste(*supp)) {
		*c = carre_get_suiv((*c));
		carre_free(supp);
		*supp = *c;
	}

	(*c) = NULL;
}

/* Structure d'une pièce (qui est une liste de Carré avec comme point de référence le bas gauche de la pièce)*/
typedef struct piece {
	Carre* liste_carre; /* Premier carré de la liste */
} Piece;

/* Création d'une Piece */
Piece* creer_piece () {
	Piece** p = malloc(sizeof(Piece*) * 21);
	p->liste_carre = NULL;
}

/* Supprime la pièce courante de la liste (si elle n'est pas NULL) */
void liste_piece_suppr_elem(Carre** c) {
	if (!carre_hors_liste(*c)) {
		Carre* suiv = carre_get_suiv((*c));
		carre_detruire(c);
		*c = suiv;
	}
}

