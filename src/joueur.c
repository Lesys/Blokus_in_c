#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/joueur.h"
/*#include "../include/commun.h"*/

/* Récupère le pseudo du joueur */
char* joueur_pseudo(Joueur* j) {
	return j->pseudo;
}

/* Récupère la Couleur du joueur */
Couleur joueur_couleur(Joueur* j) {
	return j->couleur;
}

/* Récupère le score du joueur */
int joueur_score(Joueur* j) {
	return j->score;
}

/* Récupère la liste des pièces du joueur */
Piece** joueur_liste_piece(Joueur* j) {
	return j->liste_piece;
}

/* Récupère le joueur suivant */
Joueur* joueur_suivant(Joueur* j) {
	return j->suiv;
}

/* Vérifie qu'il y a un nombre correct de joueur */
int verif_nb_joueur(int nb, int min, int max) {
	return (nb >= min && nb <= max);
}

/* Alloue "nb_joueur" Joueurs, demande les pseudos et ajoute chaque joueur à la suite d'un autre. Retourne le 1er joueur créé (qui sera le joueur BLEU) */
/* Les paramètres optionnels seront le nombre de BOT ainsi que le nombre de joueurs en réseau (A FAIRE PLUS TARD) */
Joueur* joueur_liste_creation(int nb_joueur, ...) {

	if (!verif_nb_joueur(nb_joueur, 2, 4)) {
		printf("Le nombre de joueur ne correspond pas (%d au lieu d'être entre %d et %d)", nb_joueur, 2, 4);
		return NULL;
	}

	int i = 0;
	Joueur* first, *tmp;
	Couleur c = BLEU;	/* Le premier joueur est BLEU */

	/* Création du premier joueur */
	first = joueur_creation(c);
	first->suiv = first->prec = first;
	tmp = first;
	c++;

	/* Le premier joueur est créé, il ne faut donc pas le créer de nouveau. On commence à 1 */
	for (i = 1; i < nb_joueur; i++) {
		tmp->suiv = joueur_creation(c);
		(tmp->suiv)->suiv = first;
		(tmp->suiv)->prec = tmp;

		tmp = tmp->suiv;

		/* Augmente la couleur d'un cran */
		c++;
	}

	/* Création de BOT une fois qu'il n'y a plus de joueur humains à créer */
/*	for (; i < NB_JOUEUR_MAX; i++) {
		tmp->suiv = joueur_creation(c, BOT);
		tmp = tmp->suiv;
*/
		/* Augmente la couleur d'un cran */
/*		c++;
	}*/
	/* Attribue le 1er joueur en tant que suivant du dernier */
	first->prec = tmp;

	return first;
}

/* Crée un joueur. Les paramètres optionnels seront le type de joueur (LOCAL par défaut) */
Joueur* joueur_creation(Couleur c, ...) {
	Joueur* j = malloc(sizeof(Joueur));
	j->pseudo = malloc(sizeof(char) * 40);

	printf("Joueur %s, veuillez indiquer votre pseudo: ", couleur_tostring(c));
/*	scanf("%*[^\n]%*c", j->pseudo);*/
/*	scanf("%s", j->pseudo);*/

	/* Réalloue la bonne taille pour le pseudo */
	j->pseudo = realloc(j->pseudo, sizeof(char) * strlen(j->pseudo));

	j->couleur = c;

	j->type = LOCAL;

	j->suiv = malloc(sizeof(Joueur*));

	j->prec = malloc(sizeof(Joueur*));

/*	j->liste_piece = piece_liste_creation();*/
	j->liste_piece = NULL;

	return j;
}

int joueur_hors_liste(Joueur* j) {
	return (j == NULL);
}

/* Libère un Joueur */
void joueur_free(Joueur** j) {
	if (!joueur_hors_liste(*j)) {
		(*j)->suiv = NULL;
		(*j)->prec = NULL;

		if ((*j)->pseudo != NULL)
			free((*j)->pseudo);
		(*j)->pseudo = NULL;

		free(*j);
		(*j) = NULL;
	}
}

/* Détruit uniquement le joueur actuel et renvoit le joueur suivant */
Joueur* joueur_detruire(Joueur** j) {
	Joueur* suiv;

	if ((*j)->suiv == *j)
		suiv = NULL;
	else {
		suiv = (*j)->suiv;

		/* Redéfini les joueurs suivants et précédents */
		suiv->prec = (*j)->prec;
		(*j)->prec->suiv = suiv;
	}

	joueur_free(j);

	return suiv;
}

/* Détruit tous les joueurs */
void joueur_liste_detruire(Joueur** j) {
	Joueur* supp;
	supp = *j;

	while (!joueur_hors_liste(*j)) {
		(*j) = joueur_detruire(j);
		/*(*j) = NULL;
		(*j) = supp;*/
	}

	(*j) = NULL;
}

