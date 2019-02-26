#include "../include/gestion_tour.h"
#include "../include/joueur.h"

#include "stdio.h"
#include "stdlib.h"

int main()
{
  Joueur* joueur = joueur_liste_creation(2);

  Piece* piece = demander_piece(joueur);

  carre_afficher(piece_liste_carre(piece));

  demander_orientation(piece);

  carre_afficher(piece_liste_carre(piece));

  return 0;
}
