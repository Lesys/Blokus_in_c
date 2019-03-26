#ifndef AFFICHAGE_SDL_H
#define AFFICHAGE_SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "couleur.h"
#include "commun.h"
#include "sprite.h"
#include "carre.h"
#include "joueur.h"

typedef struct ressources {
	Sprite * carre_vide;
	Sprite * carre_rouge;
	Sprite * carre_jaune;
	Sprite * carre_bleu;
	Sprite * carre_vert;
	TTF_Font * police_m;
	TTF_Font * police_p;
	Sprite * fond_score;
	Sprite * fond_tour;
	Sprite * bouton;
	Sprite * bouton_hover;
	Sprite * bouton_petit;
	Sprite * bouton_petit_hover;
	Sprite * fond_resultats;
	Sprite * fond_titres;
	Sprite * fond_config;
	Sprite * fond_saisie;
	Sprite * tapis_rouge;
	Sprite * tapis_vert;
	Sprite * tapis_jaune;
	Sprite * tapis_bleu;
	Sprite * fond_plateau;
	SDL_Color blanc;
	SDL_Color jaune;
	SDL_Color vert;
	SDL_Color bleu;
	SDL_Color rouge;
} Ressources;

typedef struct reserves {
	int taille_carre;
	Piece * vert[LARG_T_VJ][LONG_T_VJ];
	Piece * jaune[LARG_T_VJ][LONG_T_VJ];
	Piece * bleu[LARG_T_BR][LONG_T_BR];
	Piece * rouge[LARG_T_BR][LONG_T_BR];
	int pos_vert_x;
	int pos_vert_y;
	int pos_jaune_x;
	int pos_jaune_y;
	int pos_bleu_x;
	int pos_bleu_y;
	int pos_rouge_x;
	int pos_rouge_y;
} Reserves;

typedef enum type_bouton {
	ABANDONNER,
	CONTINUER,
	QUITTER_PARTIE,
	JOUER,
	QUITTER_JEU,
	NB_JOUEURS_2,
	NB_JOUEURS_3,
	NB_JOUEURS_4,
	TYPE_JOUEUR_LOCAL,
	TYPE_JOUEUR_DISTANT,
	TYPE_JOUEUR_BOT,
        CREER_PARTIE,
        REJOINDRE_PARTIE,
        RETOUR
} Type_bouton;

typedef struct bouton {
	Type_bouton type;
	int x_gauche;
	int x_droite;
	int y_haut;
	int y_bas;
} Bouton;


int init_affichage_sdl(); // Appelé par sdl_init() chargement des sprites dans le struct sprite qui sera en variable globale

void free_affichage_sdl();

void afficher_plateau_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]); // Affichage plateau au millieu de l'écran

int curs_hover_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], int *x, int *y);

Reserves * init_afficher_pieces_dispo_sdl(Joueur * j);

void afficher_pieces_dispo_sdl(Reserves * r, Joueur * j, Piece * p);

void free_afficher_pieces_dispo_sdl(Reserves ** r);

Piece * curs_hover_piece(Reserves * r, Couleur couleur);

void afficher_scores_sdl(Joueur * j); // Affichage scores en haut à gauche écran

void afficher_tour_sdl(Joueur * j);

Bouton * init_bouton_sdl(Type_bouton b);

void afficher_bouton_sdl(Bouton * b);

void free_bouton_sdl(Bouton ** b);

void afficher_resultats_sdl(Joueur * j); // Affichage popup avec les scores + continuer/quitter

void afficher_titres_sdl();

void afficher_nb_joueurs_sdl();

void afficher_type_joueur_sdl();

void afficher_saisie_pseudo_sdl(Joueur * j);

void afficher_choix_type_partie_sdl();

void afficher_attente_connexion_sdl();

void afficher_attente_pseudo_sdl();

void afficher_saisie_adresse_sdl(char * str);

void afficher_saisie_pseudo_distant_sdl(Joueur * j);

int curs_hover_bouton(Bouton * b);

#endif
