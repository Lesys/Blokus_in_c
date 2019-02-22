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

    printf("Test affichage du plateau :\n\n");
    afficher_plateau(pl);

    // Test pièces restantes
    printf("Test affichage pièces restantes :\n\n");
    Joueur * j = joueur_creation(ROUGE);
    afficher_pieces_dispo(j);
}
