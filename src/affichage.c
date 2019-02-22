#include <stdio.h>

#include "../include/commun.h"
#include "../include/affichage.h"
#include "../include/joueur.h"
#include "../include/carre.h"

void afficher_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {
    
    for (int i = 0; i < TAILLE_PLATEAU; i++) {
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
}

void afficher_pieces_dispo(Joueur* j) {

    return;
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
