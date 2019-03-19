/**
	\file joueur.c
	\brief Fichier de fonctions pour accesseurs et de modifications pour un Joueur
	\author WIDMER Alexis
	\version 1.0
	\date 05/03/2019

	Ce fichier permet d'avoir accès aux attributs d'un Joueur, de le créer ou de le supprimer.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/joueur.h"
#include "../include/carre.h"
#include "../include/commun.h"

/* Récupère le pseudo du joueur */
/**
	\fn char* joueur_pseudo(Joueur* j);
	\brief Permet de récupérer le pseudo d'un Joueur*

	\param j Le Joueur* dont on veut récupérer le pseudo
	\return Le pseudo du Joueur*
*/
char* joueur_pseudo(Joueur* j) {
	return j->pseudo;
}

/* Récupère la Couleur du joueur */
/**
	\fn Couleur joueur_couleur(Joueur* j);
	\brief Permet de récupérer la couleur d'un Joueur* (énumération)

	\param j Le Joueur* dont on veut récupérer la couleur
	\return La couleur du Joueur*
*/
Couleur joueur_couleur(Joueur* j) {
	return j->couleur;
}

/* Récupère le score du joueur */
/**
	\fn int joueur_score(Joueur* j);
	\brief Permet de récupérer le score d'un Joueur*

	\param j Le Joueur* dont on veut récupérer le score
	\return Le score du Joueur*
*/
int joueur_score(Joueur* j) {
	return j->score;
}

/* Récupère la liste des pièces du joueur */
/**
	\fn Piece* joueur_liste_piece(Joueur* j);
	\brief Permet de récupérer la liste de Piece d'un Joueur*

	\param j Le Joueur* dont on veut récupérer la liste de Piece
	\return La première Piece* du Joueur*
*/
Piece* joueur_liste_piece(Joueur* j) {
	return j->liste_piece;
}

/* Récupère le joueur suivant */
/**
	\fn Joueur* joueur_suivant(Joueur* j);
	\brief Permet de récupérer le prochain Joueur* à jouer

	\param j Le Joueur* dont on veut récupérer le Joueur suivant
	\return Le Joueur* suivant
*/
Joueur* joueur_suivant(Joueur* j) {
	return j->suiv;
}

/**
	\fn int joueur_a_abandonne(Joueur* j);
	\brief Permet de savoir si le Joueur* a abandonné ou non

	\param j Le Joueur* dont on veut récupérer la valeur d'abandon
	\return VRAI si le Joueur* a abandonné, FAUX sinon
*/
int joueur_a_abandonne(Joueur* j) {
	return j->abandon;
}

/**
	\fn Joueur* joueur_abandonne(Joueur* j);
	\brief Fait abandonner un Joueur

	\param j Le Joueur* qu'on veut faire abandonner
*/
void joueur_abandonne(Joueur* j) {
	j->abandon = 1;
}

/* Vérifie qu'il y a un nombre correct de joueur */
/**
	\fn int verif_nb_joueur(int nb);
	\brief Vérifie si le nombre de joueur est correct (avec les variables en DEFINE)

	\param nb Le nombre de joueur saisi par l'utilisateur
	\return VRAI si le nombre de joueur saisi est correct, FAUX sinon
*/
int verif_nb_joueur(int nb) {
	return (nb >= NB_JOUEUR_MIN && nb <= NB_JOUEUR_MAX);
}

/* Renvoie le nombre de pièces qui sont encore dans la liste */
/**
	\fn int joueur_nb_piece_restantes(Joueur* j);
	\brief Renvoie le nombre de Piece qui reste au Joueur passé en paramètre

	\param j Le Joueur* dont on veut connaître le nombre de Piece restantes
	\return Le nombre de Piece restantes pour le Joueur*
*/
int joueur_nb_piece_restantes(Joueur* j) {
	int i = 0;
	Piece* init = joueur_liste_piece(j);
	Piece* suiv = init;;

	if (init != NULL)
		/* Compte le nombre de pièces dans la liste */
		for (i = 1; ((suiv = piece_suivant(suiv)) != init); i++);

	return i;
}

/* Alloue "nb_joueur" Joueurs, demande les pseudos et ajoute chaque joueur à la suite d'un autre. Retourne le 1er joueur créé (qui sera le joueur BLEU) */
/* Les paramètres optionnels seront le nombre de BOT ainsi que le nombre de joueurs en réseau (A FAIRE PLUS TARD) */
/**
	\fn Joueur* joueur_liste_creation(int nb_joueur, ...);
	\brief Crée la liste des Joueur en fonction du nombre passé en paramètre

	\param nb_joueur Le nomber de joueur qu'on souhaite créer
	\return Le premier Joueur* (les autres Joueur sont accessibles via joueur_suivant). NULL Si le nombre de joueur n'est pas correct.
*/
Joueur* joueur_liste_creation(int nb_joueur, ...) {

	if (!verif_nb_joueur(nb_joueur)) {
		printf("Le nombre de joueur ne correspond pas (%d au lieu d'être entre %d et %d)", nb_joueur, NB_JOUEUR_MIN, NB_JOUEUR_MAX);
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

/* Fonction permettant de réinitialiser la liste de Joueur pour permettre de refaire une manche avec les mêmes pseudos */
/**
	\fn void joueur_liste_reinit(Joueur* j);
	\brief Réinitialise les Joueur (au début d'une manche par exemple) sans modifier le score

	\param j Le Joueur* qu'on veut réinisialiser (tous les Joueur sont réinitialisés)
*/
void joueur_liste_reinit(Joueur* j) {
	Joueur* first = j;

	/* Pour tous les Joueur */
	do {
		j->abandon = 0;

                liste_piece_detruire(&(j->liste_piece));

		j->liste_piece = piece_liste_creation();

		j = joueur_suivant(j);
	} while (first != j);
}

/* Crée un joueur. Les paramètres optionnels seront le type de joueur (LOCAL par défaut) */
/**
	\fn Joueur* joueur_creation(Couleur c, ...);
	\brief Crée un Joueur en demandant le pseudo.

	\param c La Couleur du joueur à créer
	\return Le Joueur* crée
*/
Joueur* joueur_creation(Couleur c, ...) {
	Joueur* j = malloc(sizeof(Joueur));
	j->pseudo = malloc(sizeof(char) * TAILLE_PSEUDO);
	j->pseudo[0] = '\0';

/*	printf("Joueur %s, veuillez indiquer votre pseudo: ", couleur_tostring(c));*/
/*	scanf("%*[^\n]%*c", j->pseudo);*/
/*	scanf("%15s", j->pseudo);

	while (getchar() != '\n');*/

	/* Réalloue la bonne taille pour le pseudo */
/*	j->pseudo = realloc(j->pseudo, sizeof(char) * (strlen(j->pseudo) + 1));*/

	j->couleur = c;

	j->type = LOCAL;

	j->suiv = NULL;/*malloc(sizeof(Joueur*));*/

	j->prec = NULL;/*malloc(sizeof(Joueur*));*/

	j->liste_piece = piece_liste_creation();
/*	j->liste_piece = NULL;*/

	j->abandon = 0;

	j->score = 0;

	return j;
}

/**
	\fn int joueur_hors_liste(Joueur* j);
	\brief Regarde si le Joueur existe ou non

	\param j Le Joueur* dont on veut savoir s'il existe ou non
	\return Renvoie VRAI si le Joueur n'existe pas (== est NULL), FAUX sinon
*/
int joueur_hors_liste(Joueur* j) {
	return (j == NULL);
}

/* Libère un Joueur */
/**
	\fn void joueur_free(Joueur** j);
	\brief Libère un joueur (et tous ses attributs)

	\param j L'adresse du Joueur* qu'on souhaite libérer
*/
void joueur_free(Joueur** j) {
	if (!joueur_hors_liste(*j)) {
		(*j)->suiv = NULL;
		(*j)->prec = NULL;

		if ((*j)->pseudo != NULL)
			free((*j)->pseudo);
		(*j)->pseudo = NULL;

		liste_piece_detruire(&((*j)->liste_piece));

		free(*j);
		(*j) = NULL;
	}
}

/* Détruit uniquement le joueur actuel et renvoit le joueur suivant */
/**
	\fn Joueur* joueur_detruire(Joueur** j);
	\brief Détruit un Joueur

	\param j L'adresse du Joueur* qu'on souhaite détruire
	\return Le Joueur* suivant (pour refaire la liste)
*/
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
/**
	\fn void joueur_liste_detruire(Joueur** j);
	\brief Détruit tous les Joueur

	\param j L'adresse du Joueur* qu'on souhaite détruire (détruit TOUS les Joueur)
*/
void joueur_liste_detruire(Joueur** j) {

	while (!joueur_hors_liste(*j)) {
		(*j) = joueur_detruire(j);
	}

	(*j) = NULL;
}

