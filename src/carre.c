#include <stdio.h>
#include <stdlib.h>
#include "../include/carre.h"
#include "../include/piece.h"

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

	if (!carre_hors_liste(c)) {
		carre_reinit_tab(tab);

		/* Tant que le Carre qu'on regarde n'est pas le premier qu'on a eu (== la boucle n'est pas terminée) */
		do {
			tab[4 - c->x][c->y] = '*'; /* Pour centrer la pièce en bas à gauche */
		} while ((c = carre_get_suiv(c)) != init);

		/* Affiche le tableau avec des ' ' (pas de Carre) ou des '*' (un Carre) */
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 5; j++)
				putchar(tab[i][j]);
			putchar('\n');
		}
	}
}


/* Libère un Carre */
void carre_free(Carre** c) {
	if (!carre_hors_liste(carre_get_suiv(*c)))
		(*c)->suiv = NULL;

	free(*c);
	(*c) = NULL;
}

/* Detruit une pièce entière */
void carre_detruire(Carre** c) {
	Carre** supp;
	(*supp) = (*c)->suiv;

	while (!carre_hors_liste((*supp))) {
/*		carre_afficher((*c));*/
/*printf("Destruction carre\n");*/
		(*supp) = carre_get_suiv((*c));


/*	if (carre_hors_liste((*c)))
		printf("c hors liste\n");
	else
		printf("c pas hors liste\n");*/

		if ((*c)->suiv == (*c))
			(*c) = NULL;
		else
			(*c)->suiv = (*supp)->suiv;

		carre_free(supp);

		(*supp) = (*c);
/*	if (carre_hors_liste((*supp)))
		printf("supp hors liste\n");
	else
		printf("supp pas hors liste\n");*/
	}


	(*c) = NULL;
}

/* Création d'une liste de Piece */
Piece** piece_liste_creation () {
	Piece** p = malloc(sizeof(Piece*) * 21); /* Alloue l'espace pour 21 pointeurs de Piece */
	Piece** first = p;

	(*p) = malloc(sizeof(Piece)); /* Alloue l'espace pour une Piece */
	(*p)->liste_carre = piece_petit_carre(); /* Récupère l'adresse de la liste de carre */
/*	carre_afficher((*p)->liste_carre);*/
	p++; /* Change le pointeur */

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_petite_barre();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_petit_coin();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_petite_moy_barre();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_carre();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_petit_t();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_moy_grande_barre();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_petit_l();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_petit_s();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_grand_l();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_grand_t();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_equerre();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_long_s();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_grand_z();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_grande_barre();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_pouce();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_escalier();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_petit_u();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_autre_1();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_croix();
	p++;

	(*p) = malloc(sizeof(Piece));
	(*p)->liste_carre = piece_autre_2();

	return first;
}

/* Supprime la pièce courante de la liste (si elle n'est pas NULL) */
void liste_piece_suppr_elem(Carre** c) {
	if (!carre_hors_liste(*c)) {
/*		printf("Destruction carres\n");*/
		Carre* suiv = carre_get_suiv(*c);
		carre_detruire(c);


		if (!carre_hors_liste(*c))
			free(*c);

		(*c) = suiv;
	}
}

void liste_piece_detruire(Piece*** p) {
	int i = 0;

	if ((*p) != NULL) {
		for (i = 0; i < 21; i++) {
/*			printf("Destruction piece %d\n", i);*/
/*		carre_afficher((*((*p) + i))->liste_carre);*/
			liste_piece_suppr_elem(&((*((*p) + i))->liste_carre));
			free(*((*p) + i));
		}

		free(*p);
		(*p) = NULL;
	}
}
