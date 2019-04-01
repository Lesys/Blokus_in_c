/**
 * \file affichage.c
 * \brief Fonctions d'affichages
 * \details Diverses fonctions d'affichage dans le terminal pour
    le jeu du blokus
 * \author BASTIDE Robin
 */

#include <stdio.h>
#include <string.h>

#include "../include/commun.h"
#include "../include/affichage.h"
#include "../include/joueur.h"
#include "../include/carre.h"

/**
 * \fn void afficher_str_couleur(Couleur couleur, char * str)
 * \brief Affiche une chaine de caractères en couleur
 * \details Affiche une chaine de caractères en couleur, si la couleur
    est "VIDE" ou n'est pas reconnue alors la fonction affiche deux espaces
 * \param couleur Couleur du texte à afficher
 * \param str Chaine à afficher
 */
void afficher_str_couleur(Couleur couleur, char * str) {

    switch(couleur) {

        case BLEU:
            printf(COULEUR_BLEU);
            printf("%s", str);
            printf(FIN_COULEUR);
            break;
        case JAUNE:
            printf(COULEUR_JAUNE);
            printf("%s", str);
            printf(FIN_COULEUR);
            break;
        case ROUGE:
            printf(COULEUR_ROUGE);
            printf("%s", str);
            printf(FIN_COULEUR);
            break;
        case VERT:
            printf(COULEUR_VERT);
            printf("%s", str);
            printf(FIN_COULEUR);
            break;
        default:
            printf("  ");
    }
}

/**
 * \fn void afficher_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU])
 * \brief Affiche le plateau de jeu
 * \details Affiche le plateau de jeu ainsi que les numéros de ligne et de
    colonnes pour mieux se repérer
 * \param pl Matrice de couleur représentant le plateau
 */
void afficher_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {

    for (int y = TAILLE_PLATEAU - 1; y >= 0; y--) {
        printf("%3d ", y + 1);
        for (int x = 0; x < TAILLE_PLATEAU; x++) {
            afficher_str_couleur(pl[x][y], "██");
        }
        printf("\n");
    }


    printf("    ");
    for (int i = 1; i <= TAILLE_PLATEAU; i++) {
        printf("%2d", i);
    }
    printf("\n");
}

/**
 * \fn void afficher_pieces_dispo(Joueur * j)
 * \brief Affiche les pièces disponibles d'un joueur
 * \details Affiche les pièces disponibles d'un joueur ainsi que des
    numéros pour faciliter le choix d'une pièce, de plus les pièces
    sont de la couleur du joueur
 * \param j Structure Joueur dont on veut afficher les pièces
 */
void afficher_pieces_dispo(Joueur * j) {

    int n = joueur_nb_piece_restantes(j);
    Piece* l = joueur_liste_piece(j);
    Couleur couleur = joueur_couleur(j);

    // Contient une rangée de 4 pièces
    // Chaque pièce est représenté dans une matrice [5][5]
    Couleur rangee[4][5][5] = {0};

    // Représente le numéro de la pièce en cours
    int i = 0;

    // Pour chaque rangée
    for (int r = 0; i < n; r++) {

        // Représentation des pièces dans leur matrice
        for (int j = 0; j < 4; j++) {
            if (i < n) {
                Carre * init = piece_liste_carre(l);
                Carre * c = init;
                do {
	            rangee[j][carre_get_x(c)][carre_get_y(c)] = couleur;
                } while ((c = carre_get_suiv(c)) != init);
                l = piece_suivant(l);
            }
            i++;
        }

        i -= 4;

        // Affichage de la rangée
        for (int y = 4; y >= 0; y--) {
            for (int p = 0; p < 4; p++) {

                if (y == 3 && i < n) {
                    printf("%2d) ", ++i);
                }
                else {
                    printf("    ");
                }

                for (int x = 0; x < 5; x++) {
                    afficher_str_couleur(rangee[p][x][y], "██");
                    rangee[p][x][y] = 0;
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}


/**
 * \fn void afficher_choix_orientation(Piece * p, Joueur * j)
 * \brief Affiche les orientations possibles pour une pièce
 * \details Affiche les orientations possibles d'une pièce ainsi que des
    numéros pour faciliter le choix d'une orientation
 * \param p Structure pièce dont on veut afficher les orientations
 * \param j Joueur qui doit choisir l'orientation (pour la couleur)
 */
void afficher_choix_orientation(Piece * p, Joueur * j) {

    // Matrice qui contiendra une représentation de la pièce
    Couleur mp[5][5] = {0};
    Carre * init = piece_liste_carre(p);
    Carre * c = init;
    Couleur couleur = joueur_couleur(j);

    // Affichage de toutes les orientations de la pièce en une ligne
    for (int y = 4; y >= 0; y--) {
        for (int n = 1; n <= 4; n++) {
            if (y == 3) {
                printf("%d) ", n);
            }
            else {
                printf("   ");
            }


            // Représentation de la pièce dans la matrice
            do {
                mp[carre_get_x(c)][carre_get_y(c)] = couleur;
            } while ((c = carre_get_suiv(c)) != init);

            // Affichage de la matrice
            for (int x = 0; x < 5; x++) {
                afficher_str_couleur(mp[x][y], "██");
            }

            // Remise à 0 de la matrice
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    mp[i][j] = 0;
                }
            }

            piece_pivoter(1, c);
            printf(" ");
        }
        printf("\n");
    }
}

/**
 * \fn void afficher_scores(Joueur * j)
 * \brief Affiche les scores
 * \details Affiche les scores actuels en mettant à la bonne couleur les
    pseudos des joueurs
 * \param j Liste de structures joueurs à afficher
 */
void afficher_scores(Joueur * j) {

    // Variable pour garder le premier joueur
    Joueur * pj = j;

    // Affichage des noms avec la bonne couleur
    do {
        printf("|");
        switch (joueur_couleur(j)) {

            case BLEU:
                printf(COULEUR_BLEU);
                break;
            case JAUNE:
                printf(COULEUR_JAUNE);
                break;
            case ROUGE:
                printf(COULEUR_ROUGE);
                break;
            case VERT:
                printf(COULEUR_VERT);
                break;
            default:
                break;
        }
        int marges = (TAILLE_PSEUDO - strlen(joueur_pseudo(j))) / 2;
        int decalage = strlen(joueur_pseudo(j))%2;
        printf("%*s%s%*s", marges + decalage, "", joueur_pseudo(j), marges, "");
        printf(FIN_COULEUR);
        j = joueur_suivant(j);
    } while (j != pj);
    printf("|\n");

    // Affichage de ligne avec juste les barres
    printf("|%*s|%*s|%*s|%*s|\n", TAILLE_PSEUDO, "", TAILLE_PSEUDO, "", TAILLE_PSEUDO, "", TAILLE_PSEUDO, "");

    // Affichage des scores
    do {
        printf("|");
        char score[TAILLE_PSEUDO];
        sprintf(score, "%d", joueur_score(j));
        int marges = (TAILLE_PSEUDO - strlen(score)) / 2;
        int decalage = strlen(score)%2;
        printf("%*s%s%*s", marges + decalage, "", score, marges, "");
        j = joueur_suivant(j);
    } while (j != pj);
    printf("|\n");
}

/**
 * \fn void afficher_resultats(Joueur * j)
 * \brief Affiche les résultats
 * \details Affiche les résultats de la partie en mettant à la bonne couleur les
    pseudos des joueurs et en gérant les ex aequo
 * \param j Liste de structures joueurs à afficher
 */
void afficher_resultats(Joueur* j) {


    // Variable pour garder le premier joueur
    Joueur * pj = j;
    // Variable pour savoir si le joueur a été placé dans le classement
    int p = 0;
    // Variable pour le décalage des joueurs
    Joueur * tmp1;
    Joueur * tmp2;

    // Tri des joueurs par ordre croissant de score dans un tableau
    Joueur * classement[4] = {NULL};

    do {
        p = 0;
        for (int i = 0; i < 4 && !p; i++) {
            if (classement[i] == NULL) {
                classement[i] = j;
                p++;
            }
            else if (joueur_score(j) > joueur_score(classement[i])) {
                // Placement du joueur et décalage des autres
                tmp1 = j;
                while (i < 4) {
                    tmp2 = classement[i];
                    classement[i] = tmp1;
                    tmp1 = tmp2;
                    i++;
                }
            }
        }
        j = joueur_suivant(j);
    } while (j != pj);

    // Affichage des résultats
    printf("%*s%s%*s\n", 12, "", "RESULTATS DE LA PARTIE", 12, "");
    printf("\n");
    for (int i = 0; i < 4 && classement[i]; i++) {

        switch (joueur_couleur(classement[i])) {

            case BLEU:
                printf(COULEUR_BLEU);
                break;
            case JAUNE:
                printf(COULEUR_JAUNE);
                break;
            case ROUGE:
                printf(COULEUR_ROUGE);
                break;
            case VERT:
                printf(COULEUR_VERT);
                break;
            default:
                break;
        }

        // Gestion des égalités
        int r = i; // rang réel
        while (r-1 >= 0 && joueur_score(classement[r]) == joueur_score(classement[r-1])) {
            r--;
        }
        r++; // +1 pour l'affichage

        // Affichage rang
        char rang[TAILLE_PSEUDO];
        if (r == 1) {
            sprintf(rang, "Vainqueur");
        }
        else {
            sprintf(rang, "%deme", r);
        }
        int marges = (TAILLE_PSEUDO/2 - strlen(rang)) / 2;
        int decalage = strlen(rang)%2;
        printf("%*s%s%*s", marges + decalage, "", rang, marges, "");

        // Affichage ex aequo ou non
        if (r != i+1) {
            printf(" ex aequo ");
        }
        else {
            printf("          ");
        }

        // Affichage pseudo
        char pseudo[TAILLE_PSEUDO];
        strcpy(pseudo,joueur_pseudo(classement[i]));
        marges = (TAILLE_PSEUDO - strlen(pseudo)) / 2;
        decalage = strlen(pseudo)%2;
        printf("%*s%s%*s", marges + decalage, "", pseudo, marges, "");

        // Affichage score
        char score[10];
        sprintf(score, "%d", joueur_score(classement[i]));
        marges = (10 - strlen(score)) / 2;
        decalage = strlen(score)%2;
        printf("%*s%s%*s", marges + decalage, "", score, marges, "");
        printf(FIN_COULEUR);
        printf("\n");
    }
}
