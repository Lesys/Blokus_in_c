#include <stdio.h>

#include "../include/commun.h"
#include "../include/affichage.h"
#include "../include/joueur.h"
#include "../include/carre.h"

void afficher_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {
    
    for (int i = 0; i < TAILLE_PLATEAU; i++) {
        printf("%3d ", TAILLE_PLATEAU - i);
        for (int j = 0; j < TAILLE_PLATEAU; j++) {
            switch(pl[i][j]) {
             
                case BLEU:
                    printf(COULEUR_BLEU);
                    printf("██");
                    printf(FIN_COULEUR);
                    break;
                case JAUNE:
                    printf(COULEUR_JAUNE);
                    printf("██");
                    printf(FIN_COULEUR);
                    break;
                case ROUGE:
                    printf(COULEUR_ROUGE);
                    printf("██");
                    printf(FIN_COULEUR);
                    break;
                case VERT:
                    printf(COULEUR_VERT);
                    printf("██");
                    printf(FIN_COULEUR);
                    break;
                case VIDE:
                default:
                    printf("  ");
            }
        }
        printf("\n");
    }


    printf("    ");
    for (int i = 1; i <= TAILLE_PLATEAU; i++) {
        printf("%2d", i);
    }
    printf("\n");
}

void afficher_pieces_dispo(Joueur* j) {

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
                Carre * init = l->liste_carre;
                Carre * c = init;
                do {
	            rangee[j][4 - c->x][c->y] = couleur;
                } while ((c = carre_get_suiv(c)) != init);
                l = piece_suivant(l);
            }
            i++;
        }

        i -= 4;

        // Affichage de la rangée
        for (int y = 0; y < 5; y++) {
            for (int p = 0; p < 4; p++) {
                
                if (y == 1 && i < n) {
                    printf("%2d) ", ++i);
                }
                else {
                    printf("    ");
                }

                for (int x = 0; x < 5; x++) {
                    switch(rangee[p][y][x]) {
                     
                        case BLEU:
                            printf(COULEUR_BLEU);
                            printf("██");
                            printf(FIN_COULEUR);
                            break;
                        case JAUNE:
                            printf(COULEUR_JAUNE);
                            printf("██");
                            printf(FIN_COULEUR);
                            break;
                        case ROUGE:
                            printf(COULEUR_ROUGE);
                            printf("██");
                            printf(FIN_COULEUR);
                            break;
                        case VERT:
                            printf(COULEUR_VERT);
                            printf("██");
                            printf(FIN_COULEUR);
                            break;
                        case VIDE:
                        default:
                            printf("  ");
                    }
                    rangee[p][y][x] = 0;
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}

void afficher_choix_orientation(Piece* p) {

    return;
}

void afficher_scores(Joueur* j) {

    return;
}

void afficher_resultats(Joueur* j) {

    return;
}
