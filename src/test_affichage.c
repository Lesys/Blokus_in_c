#include "../include/affichage.h"

int main() {
    Couleur pl[TP][TP] = {0};

    // Test des différentes couleurs
    pl[0][0] = ROUGE;
    pl[TP-1][0] = JAUNE;
    pl[0][TP-1] = VERT;
    pl[TP-1][TP-1] = BLEU;

    // Test d'une valeur non attendue
    pl[10][10] = 1234;

    afficher_plateau(pl);
}
