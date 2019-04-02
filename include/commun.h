#ifndef COMMUN_H
#define COMMUN_H

#define TAILLE_PLATEAU 20
#define TAILLE_PSEUDO 15
#define NB_PIECES 21
#define NB_JOUEUR_MIN 2
#define NB_JOUEUR_MAX 4

#define L_FENETRE 1280
#define H_FENETRE 720
#define LARG_T_VJ 38
#define LONG_T_VJ 9
#define LARG_T_BR 18
#define LONG_T_BR 18

#define BLUE_X 0
#define BLUE_Y 0
#define RED_X TAILLE_PLATEAU - 1
#define RED_Y TAILLE_PLATEAU - 1
#define GREEN_X 0
#define GREEN_Y TAILLE_PLATEAU - 1
#define YELLOW_X TAILLE_PLATEAU - 1
#define YELLOW_Y 0

#define PORT_DEFAUT 5555
#define TAILLE_BUFF 1000

/* Define pour le BOT */
#define PROFONDEUR 5

/* Entre 0 et 9 (0 == bord, 9 == 1 case du centre) */
#define COUP_MAUVAIS 3
#define COUP_MOYEN 5
#define COUP_BON 7

#define COEF_COUP_CENTRE 4
#define COEF_COUP_BON 3
#define COEF_COUP_MOYEN 2
#define COEF_COUP_MAUVAIS 1

#define COEF_CARRES_POSES 1
#define COEF_EMPLACEMENT_PIECE 1
#define COEF_CASES_DISPO 2
#define COEF_NOUVEAUX_COINS 2
#define COEF_COINS_BLOQUES 3

#endif
