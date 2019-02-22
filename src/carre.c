#include <stdio.h>
#include <stdlib.h>
#include "../include/carre.h"
#include "../include/piece.h"
#include "../include/commun.h"

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
	Carre** supp = malloc(sizeof(Carre*));
	(*supp) = (*c);

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

	free(supp);
	supp = NULL;
	(*c) = NULL;
}

Carre* piece_liste_carre(Piece* p) {
	return p->liste_carre;
}

Piece* piece_suivant(Piece* p) {
	return p->suiv;
}

Piece* piece_precedent(Piece* p) {
	return p->prec;
}

/* Création d'une liste de Piece */
Piece* piece_liste_creation () {
	Piece* p = malloc(sizeof(Piece)); /* Alloue l'espace pour une Piece */
	Piece* first = p;

	p->liste_carre = piece_petit_carre(); /* Récupère l'adresse de la liste de carre */
/*	carre_afficher((*p)->liste_carre);*/
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv; /* Change le pointeur */

	p->liste_carre = piece_petite_barre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_coin();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petite_moy_barre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_carre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_t();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_moy_grande_barre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_l();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_s();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_grand_l();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_grand_t();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_equerre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_long_s();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_grand_z();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_grande_barre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_pouce();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_escalier();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_u();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_autre_1();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_croix();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_autre_2();
	p->suiv = first;;
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */

	return first;
}

/* Supprime la "pos"ième pièce (si elle n'est pas NULL) */
void liste_piece_suppr_elem(Piece** p) {/*, int pos) {*/
	if (p != NULL) {
		Carre** c = &((*p)->liste_carre);
/*		printf("Destruction carres\n");*/
/*		Carre* suiv = carre_get_suiv(*c);*/

		/* Réctifie l'ordre de la liste */

/*printf("Début\n");*/
		Piece* suiv;
		if ((*p) == piece_suivant(*p))
			suiv = NULL;
		else
			suiv = piece_suivant(*p);
/*printf("Milieu\n");*/
		(*p)->prec->suiv = piece_suivant(*p);
/*printf("Fin\n");*/
		(*p)->suiv->prec = piece_precedent(*p);

/*printf("Destructino\n");*/
		carre_detruire(c);

/*printf("Free de c\n");*/

		/* Libère le pointeur de la liste de Carre */
		if (!carre_hors_liste(*c))
			free(*c);

/*		(*c) = suiv;*/
		(*c) = NULL;
/*printf("Free de p\n");*/
		/* Libère la Piece */
		if ((*p) != NULL)
			free(*p);

		(*p) = suiv;

	}
}

void liste_piece_detruire(Piece** p) {
	Piece** supp = malloc(sizeof(Piece*));

	(*supp) = (*p);

	/* Tant qu'il reste une Piece à supprimer */
	while ((*supp) != NULL) {
		(*supp) = piece_suivant(*p);

		/* Si c'est la dernière Piece (== la suivante est elle-même) */
		if ((*supp) == (*p))
			(*p) = NULL;
		else
			(*p)->suiv = piece_suivant((*supp));

		/* Supprime et libère la Piece courante */
		liste_piece_suppr_elem(supp);

		(*supp) = (*p);
	}

	free(supp);
	supp = NULL;
	(*p) = NULL;
}
/*
		for (i = 0; i < NB_PIECES; i++) {*/
/*			printf("Destruction piece %d\n", i);*/
/*		carre_afficher((*((*p) + i))->liste_carre);*/
/*			liste_piece_suppr_elem(&((*((*p) + i))->liste_carre));
			free(*((*p) + i));
		}

		free(*p);
		(*p) = NULL;
	}
}*/
