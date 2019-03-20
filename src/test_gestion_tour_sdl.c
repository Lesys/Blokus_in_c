#include "../include/gestion_tour.h"
#include "../include/joueur.h"
#include "../include/affichage.h"
#include "../include/gestion_tour_sdl.h"
#include "../include/sdl.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    Joueur* j = joueur_liste_creation(4);

    Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU] = {0};

    sdl_init();

    gestion_jeu(pl, j);

    sdl_close();

    return 0;
}
