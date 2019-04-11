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
    /** \brief Texture carré vide */
    Sprite * carre_vide;
    /** \brief Texture carré rouge */
    Sprite * carre_rouge;
    /** \brief Texture carré jaune */
    Sprite * carre_jaune;
    /** \brief Texture carré bleu */
    Sprite * carre_bleu;
    /** \brief Texture carré vert */
    Sprite * carre_vert;
    /** \brief Texture carré sélection */
    Sprite * selection;
    /** \brief Texture carré sélection erreur */
    Sprite * selection_erreur;
    /** \brief Police de taille moyenne */
    TTF_Font * police_m;
    /** \brief Police de petite taille */
    TTF_Font * police_p;
    /** \brief Texture de fond de la fenetre score */
    Sprite * fond_score;
    /** \brief Texture de fond du plateau */
    Sprite * fond_tour;
    /** \brief Texture de fond d'un tour */
    Sprite * bouton;
    /** \brief Texture de fond d'un bouton lorsque le curseur est dessus */
    Sprite * bouton_hover;
    /** \brief Texture de fond d'un petit bouton */
    Sprite * bouton_petit;
    /** \brief Texture de fond d'un petit bouton lorsque le curseur est dessus */
    Sprite * bouton_petit_hover;
    /** \brief Texture de fond de la fenetre résultats */
    Sprite * fond_resultats;
    /** \brief Texture de fond de la fenetre titres */
    Sprite * fond_titres;
    /** \brief Texture de fond de la fenetre config */
    Sprite * fond_config;
    /** \brief Texture de fond de la fenetre de saisie */
    Sprite * fond_saisie;
    /** \brief Texture du tapis rouge */
    Sprite * tapis_rouge;
    /** \brief Texture du tapis vert */
    Sprite * tapis_vert;
    /** \brief Texture du tapis jaune */
    Sprite * tapis_jaune;
    /** \brief Texture du tapis bleu */
    Sprite * tapis_bleu;
    /** \brief Texture de fond du plateau */
    Sprite * fond_plateau;
    /** \brief Texture du bouton son */
    Sprite * son;
    /** \brief Texture du bouton son si selectionné */
    Sprite * son_selec;
    /** \brief Texture du bouton effet */
    Sprite * effet;
    /** \brief Texture du bouton effet si selectionné */
    Sprite * effet_selec;
    /** \brief Texture contenant les règles */
    Sprite * regles;
    /** \brief Couleur blanche */
    SDL_Color blanc;
    /** \brief Couleur jaune */
    SDL_Color jaune;
    /** \brief Couleur verte */
    SDL_Color vert;
    /** \brief Couleur bleu */
    SDL_Color bleu;
    /** \brief Couleur rouge */
    SDL_Color rouge;
} Ressources;

typedef struct reserves {
    /** \brief Réserve des pièces vertes */
    Piece * vert[LARG_T_VJ][LONG_T_VJ];
    /** \brief Réserve des pièces jaunes */
    Piece * jaune[LARG_T_VJ][LONG_T_VJ];
    /** \brief Réserve des pièces bleus */
    Piece * bleu[LARG_T_BR][LONG_T_BR];
    /** \brief Réserve des pièces rouges */
    Piece * rouge[LARG_T_BR][LONG_T_BR];
    /** \brief Position réserve verte en x */
    int pos_vert_x;
    /** \brief Position réserve verte en y */
    int pos_vert_y;
    /** \brief Position réserve jaune en x */
    int pos_jaune_x;
    /** \brief Position réserve jaune en y */
    int pos_jaune_y;
    /** \brief Position réserve bleu en x */
    int pos_bleu_x;
    /** \brief Position réserve bleu en y */
    int pos_bleu_y;
    /** \brief Position réserve rouge en x */
    int pos_rouge_x;
    /** \brief Position réserve rouge en y */
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
    CHARGER_PARTIE,
    RETOUR,
    SON,
    EFFET,
    REGLES,
    FIN,
    SAUVEGARDER
} Type_bouton;

typedef struct bouton {
    /** \brief Type du bouton */
    Type_bouton type;
    /** \brief Coordonnée de la gauche du bouton en x */
    int x_gauche;
    /** \brief Coordonnée de la droite du bouton en x */
    int x_droite;
    /** \brief Coordonnée du haut du bouton en y */
    int y_haut;
    /** \brief Coordonnée du bas du bouton en y */
    int y_bas;
} Bouton;


int init_affichage_sdl(int fullscreen); // Appelé par sdl_init() chargement des sprites dans le struct sprite qui sera en variable globale

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

void afficher_attente_connexion_sdl(char * adresse);

void afficher_attente_pseudo_sdl();

void afficher_attente_debut_sdl();

void afficher_saisie_adresse_sdl(char * str);

void afficher_saisie_pseudo_distant_sdl(char * str);

int curs_hover_bouton(Bouton * b);

void afficher_erreur_reseau();

void afficher_attente_nouvelle_partie();

void afficher_regles_sdl();

#endif
