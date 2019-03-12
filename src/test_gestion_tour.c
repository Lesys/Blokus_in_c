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
    int x, y;

    carre_afficher(piece_liste_carre(piece));

    demander_orientation(piece, joueur);

    carre_afficher(piece_liste_carre(piece));

    choisir_coordonnee(pl, piece, &x, &y, joueur);
    poser_piece(pl, piece, joueur, x, y);
    afficher_plateau(pl);

    piece = demander_piece(joueur);
    carre_afficher(piece_liste_carre(piece));
    demander_orientation(piece, joueur);
    carre_afficher(piece_liste_carre(piece));
    choisir_coordonnee(pl, piece, &x, &y, joueur);
    if(x != -1 || y != -1)
    	poser_piece(pl, piece, joueur, x, y);
    choisir_coordonnee(pl, piece, &x, &y, joueur);
    	poser_piece(pl, piece, joueur, x, y);
    afficher_plateau(pl);

    return 0;
}
