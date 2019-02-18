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
}

int carre_hors_liste(Carre* c) {
	return (c->suiv == NULL);
}

/* Libère un Carre */
void carre_free(Carre** c) {
	if (!carre_hors_liste(*c->suiv))
		*c->suiv = NULL;

	free(*c);
	*c = NULL;
}

/* Detruit une pièce entière */
void carre_detruire(Carre** c) {
	Carre** supp;
	*supp = *c;

	while (!carre_hors_liste(*supp)) {
		*c = *c->suiv;
		carre_free(supp);
		*supp = *c;
	}

	*c = NULL;
}

/* Structure d'une pièce (qui est une liste de Carré avec comme point de référence le bas gauche de la pièce)*/
/*typedef struct piece {
	Carre* liste_carre; *//* Premier carré de la liste */
/*} Piece;*/

/* Création d'une Piece */
/*Piece* creer_piece () {
	Piece* p = malloc(sizeof(Piece));
	p->liste_carre = NULL;
}*/

/* Supprime l'élément courant de la liste (s'il n'est pas NULL)	 */
void liste_piece_suppr_elem(Carre** c) {
	if (!carre_hors_liste(*c)) {
		Carre* suiv = *c->suiv;
		carre_free(c);
		*c = suiv;
	}
}
