#include <stdio.h>
#include "../include/affichage.h"
/*
#include "../include/joueur.h"
#include "../include/carre.h"
*/

void afficher_plateau(Couleur pl[TP][TP]) {
    
    for (int i = 0; i < TP; i++) {
        for (int j = 0; j < TP; j++) {
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
}

/*
void afficher_pieces_dispo(Joueur* j) {

    return;
}

void afficher_choix_orientation(Piece* p) {

    return;
}

void afficher_scores(joueur* j) {

    return;
}

void afficher_resultats(joueur* j) {

    return;
}
*/
