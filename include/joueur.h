#ifndef JOUEUR_H
#define JOUEUR_H

#include "couleur.h"
#include "carre.h"

typedef enum type_joueur {BOT = 0, LOCAL, DISTANT} Type_Joueur;

typedef struct joueur Joueur;

/* Structure d'un Joueur */
struct joueur {
	char* pseudo;
	Couleur couleur; /* Couleur du joueur */
	int score;
	Piece* liste_piece; /* Liste des pièces restantes pour le joueur */
	Type_Joueur type; /* Type de joueur (la gestion d'un tour diffère en fonction du type) */
	Joueur* suiv; /* Joueur suivant (qu'il soit un vrai joueur ou un BOT)*/
	Joueur* prec; /* Joueur précédent (pour que la destruction soit possible)*/
	int abandon; /* VRAI si le joueur a abandonné, FAUX sinon */
};

/* Procédures d'accès pour un Joueur */

/* Récupère le pseudo du joueur */
char* joueur_pseudo(Joueur*);

/* Récupère la Couleur du joueur */
Couleur joueur_couleur(Joueur*);

/* Récupère le score du joueur */
int joueur_score(Joueur*);

/* Récupère la liste des pièces du joueur */
Piece* joueur_liste_piece(Joueur*);

/* Récupère le joueur suivant */
Joueur* joueur_suivant(Joueur*);

/* Récupère le joueur suivant */
int joueur_a_abandonne(Joueur*);

/* Change le paramètre "abandon" d'un joueur et le passe à "vrai" */
void joueur_abandonne(Joueur*);

/* Vérifie qu'il y a un nombre correct de joueur */
int verif_nb_joueur(int);

/* Permet de connaître le nombre de Piece que possède un Joueur à l'instant T */
int joueur_nb_piece_restantes(Joueur*);

/* Alloue "nb_joueur" Joueurs, demande les pseudos et ajoute chaque joueur à la suite d'un autre. Retourne le 1er joueur créé (qui sera le joueur BLEU) */
Joueur* joueur_liste_creation(int, ...);

/* Crée un seul joueur (en demandant les caractéristiques */
Joueur* joueur_creation(Couleur, ...);

void joueur_liste_reinit(Joueur*);

int joueur_hors_liste(Joueur*);

/* Libère un Joueur */
void joueur_free(Joueur**);

/* Détruit uniquement le joueur actuel et renvoit le joueur suivant */
Joueur* joueur_detruire(Joueur**);

/* Détruit tous les joueurs */
void joueur_liste_detruire(Joueur**);

#endif
