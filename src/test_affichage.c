#include <stdio.h>
#include "../include/commun.h"
#include "../include/affichage.h"
#include "../include/joueur.h"

int main() {
    Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU] = {0};

    // Test des différentes couleurs
    pl[0][0] = ROUGE;
    pl[1][1] = ROUGE;
    pl[0][1] = ROUGE;
    pl[TAILLE_PLATEAU-1][0] = JAUNE;
    pl[0][TAILLE_PLATEAU-1] = VERT;
    pl[TAILLE_PLATEAU-1][TAILLE_PLATEAU-1] = BLEU;

    // Test d'une valeur non attendue
    pl[10][10] = 1234;

    printf("\nTest affichage du plateau :\n\n");
    afficher_plateau(pl);

    // Test pièces restantes
    printf("\nTest affichage pièces restantes :\n\n");
    Joueur * j = joueur_creation(ROUGE);
    afficher_pieces_dispo(j);

    // Test orientations
    printf("\nTest affichage orientations : \n\n");
    afficher_choix_orientation(piece_suivant(piece_suivant(piece_suivant(joueur_liste_piece(j)))), j);
    
    joueur_free(&j);

    // Test scores
    printf("\nTest affichage des scores :\n\n");
    Joueur * lj = joueur_liste_creation(4);
    lj->score = 965;
    lj = joueur_suivant(lj);
    lj->score = 50;
    lj = joueur_suivant(lj);
    lj->score = 30;
    lj = joueur_suivant(lj);
    lj->score = 965;
    lj = joueur_suivant(lj);
    printf("\n");
    afficher_scores(lj);
    
    // Test résultats
    printf("\nTest affichage des résultats :\n\n");
    afficher_resultats(lj);

    joueur_liste_detruire(&lj);
}
