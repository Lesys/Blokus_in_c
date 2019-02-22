#include "../include/commun.h"
#include "../include/affichage.h"

int main() {
    Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU] = {0};

    // Test des différentes couleurs
    pl[0][0] = ROUGE;
    pl[TAILLE_PLATEAU-1][0] = JAUNE;
    pl[0][TAILLE_PLATEAU-1] = VERT;
    pl[TAILLE_PLATEAU-1][TAILLE_PLATEAU-1] = BLEU;

    // Test d'une valeur non attendue
    pl[10][10] = 1234;

    afficher_plateau(pl);
}
