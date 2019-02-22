#include <stdio.h>
#include "../include/commun.h"
#include "../include/affichage.h"
#include "../include/joueur.h"

int main() {
    Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU] = {0};

    // Test des différentes couleurs
    pl[0][0] = ROUGE;
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
    afficher_choix_orientation(piece_suivant(piece_suivant(joueur_liste_piece(j))));

    // Test scores
    printf("\nTest affichage des scores :\n\n");
    Joueur * lj = joueur_liste_creation(4);
    printf("\n");
    afficher_scores(lj);
}
