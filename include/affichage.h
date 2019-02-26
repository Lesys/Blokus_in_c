#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "../include/commun.h"
#include "../include/couleur.h"
#include "../include/joueur.h"
#include "../include/carre.h"

#define COULEUR_BLEU "\x1b[34m"
#define COULEUR_JAUNE "\x1b[33m"
#define COULEUR_ROUGE "\x1b[31m"
#define COULEUR_VERT "\x1b[32m"
#define FIN_COULEUR "\x1b[0m"

void afficher_str_couleur(Couleur couleur, char * str);
void afficher_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]);
void afficher_pieces_dispo(Joueur* j);
void afficher_choix_orientation(Piece* p);
void afficher_scores(Joueur* j);
void afficher_resultats(Joueur* j);

#endif
