#include "../include/gestion_tour.h"
#include "../include/joueur.h"
#include "../include/affichage.h"

#include "stdio.h"
#include "stdlib.h"

int main()
{
    Joueur* joueur = joueur_liste_creation(2);
    Couleur pl[20][20] = {0};
    Piece* piece = demander_piece(joueur);

    carre_afficher(piece_liste_carre(piece));

    demander_orientation(piece);

    carre_afficher(piece_liste_carre(piece));

    int x, y;
    choisir_coordonnee(pl, piece, &x, &y, joueur);

    afficher_plateau(pl);

    return 0;
}
