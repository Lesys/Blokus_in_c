#ifndef CARRE_H
#define CARRE_H

/* Structure d'un carré */

/**
	\typedef Carre: un carré d'une pièce
*/
typedef struct carre Carre;

/**
	\typedef Piece: une pièce du jeu (est formée d'une suite de Carre)
*/
typedef struct piece Piece;

/**
	\struct carre
	\brief Structure d'un Carre

	Un Carre fait parti d'une Piece (une Piece est composée d'un ou plusieurs Carre).
*/
struct carre {
	/* Coordonnées x et y par rapport au point de référence d'une pièce */
	int x; /**< Coordonnées x d'un Carre (le Carre est en haut à gauche d'un tableau (visuellement)) */
	int y; /**< Coordonnées y d'un Carre (le Carre est en haut à gauche d'un tableau (visuellement)) */
	Carre* suiv; /**< Carre suivant (permet de faire une boucle sur la Piece) */

};

/* Procédures d'accès pour un carré */

/* Permet de récupérer la coordonnée x d'un Carre */
int carre_get_x (Carre*);

/* Permet de récupérer la coordonnée y d'un Carre */
int carre_get_y (Carre*);

/* Permet de récupérer le carré suivant d'une pièce */
Carre* carre_get_suiv (Carre*);

/* Renvoie VRAI si le Carre est NULL, FAUX sinon */
int carre_hors_liste(Carre*);

/* Réinitalise un tableau de char [5][5] (utilisé uniquement pour l'affichage) */
void carre_reinit_tab(char tab[5][5]);

/* Affiche un Carre* à l'écran (== une Piece) */
void carre_afficher(Carre*);

/* Libère un Carre */
void carre_free(Carre**);

/* Detruit une pièce entière */
void carre_detruire(Carre**);

/* Structure d'une pièce (qui est une liste de Carré avec comme point de référence le bas gauche de la pièce)*/

/**
	\struct piece
	\brief Structure d'une Piece

	Une Piece est constituée d'une liste de Carre (avec les coordonnées) et des Piece* suivante et précédente
*/
struct piece {
	Carre* liste_carre; /**< Premier carré de la liste */
	Piece* suiv, *prec; /**< Piece* suivante et précédente de la liste */
        int id_piece;
};

/* Renvoie VRAI si la Piece est NULL, FAUX sinon */
int piece_hors_liste(Piece*);

/* Renvoie la liste de Carre d'une Piece* */
Carre* piece_liste_carre(Piece*);

Piece* piece_copie(Piece*);

/* Renvoie la Piece* suivante */
Piece* piece_suivant(Piece*);

/* Renvoie la Piece* précédente */
Piece* piece_precedent(Piece*);

int piece_meme_orientation(Piece*, Piece*);

/* Crée les NB_PIECES Piece* dans l'ordre et crée une chaine entre elles (avec suiv et prec)*/
Piece* piece_liste_creation();

void piece_pivoter(int, Carre *);

/* Supprime tous les carrés d'une pièce */
void liste_piece_suppr_elem(Piece**);

/* Supprime une liste de Piece* */
void liste_piece_detruire(Piece**);

/* Fonction renvoyant le nombre de Carre d'une Piece */
int piece_nb_carre(Piece*);

Piece* liste_piece_copie(Piece* p);

Piece* piece_copie(Piece*);

#endif
