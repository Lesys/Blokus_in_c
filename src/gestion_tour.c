#include "../include/gestion_tour.h"
#include "../include/affichage.h"

#include "stdio.h"
#include "stdlib.h"

/* Affiche les pièces qu'il reste au joueur avec un numéro et lui demande de choisir quelle pièce jouer et attend qu'il entre le numéro */
Piece* demander_piece(Joueur* j)
{
  int nb, i;
  Piece* p = joueur_liste_piece(j);

  printf("Quelle pièce voulez-vous jouer ?\n");
  afficher_pieces_dispo(j);
  scanf("%d", &nb);

  /* tant que le joueur entre un numéro ne correspondant pas à une pièce disponible, on redemande un numéro valide */
  while((nb < 0) && (nb > joueur_nb_piece_restantes(j)))
  {
    printf("Veuillez entrer une valeur correcte\n");
    printf("Quelle pièce voulez-vous jouer ?\n");
    afficher_pieces_dispo(j);
    scanf("%d", &nb);
  }

  /* on sélectionne la bonne pièce */
  for(i = 1; i < nb; i++)
    p = piece_suivant(p);

  return p;
}

/* Fonction initialisant une matrice de taille 5*5 en la remplissant de 0 */
void initialiser_matrice(int matrice[5][5])
{
  int i, j;

  for(i = 0; i < 5; i++)
  {
    for(j = 0; j < 5; j++)
      matrice[i][j] = 0;
  }
}

/* Fonction affectant une pièce à une matrice
   remplit la matrice de 1 en fonction des coordonnées des carres constituant la pièce */
void affecter_matrice(int matrice[5][5], Carre* c)
{
  Carre* c2 = c;
  c2 = carre_get_suiv(c2);

  while(c2 != c)
  {
    matrice[carre_get_x(c2)][carre_get_y(c2)] = 1;
    c2 = carre_get_suiv(c2);
  }

  matrice[carre_get_x(c2)][carre_get_y(c2)] = 1;
}

/* Fonction permettant d'afficher une matrice 5*5

void afficher_matrice(int matrice[5][5])
{
  int i, j;

  for(i = 4; i >= 0; i--)
  {
    for(j = 0; j < 5; j++)
      printf("%d", matrice[i][j]);

    printf("\n");
  }

  printf("\n");
}
*/

/* Affiche les 4 orientations possibles de la pièce au joueur avec un numéro et attend qu'il entre un numéro */
/* modifie également les coordonnees relatives des carres constituant la piece une fois l'orientation choisie */
void demander_orientation(Piece* p)
{
  int nb;
  int min_x;
  int min_y;
  Orientation o;
  Carre* c = piece_liste_carre(p);

  afficher_choix_orientation(p);
  printf("Dans quelle orientation voulez-vous jouer la pièce ? :\n");
  scanf("%d", &nb);
  nb--;

  /* tant que le joueur n'entre pas un numéro d'orientation correct, on lui redemande un numéro valide */
  while((nb < HAUT) || (nb > GAUCHE))
  {
    printf("Veuillez entrer une orientation correcte\n");
    printf("Dans quelle orientation voulez-vous jouer la pièce ? :\n");
    afficher_choix_orientation(p);
    scanf("%d", &nb);
    nb--;
  }

  int i, j;

  /* Pour chaque orientation, on tourne la pièce de 90° un certain nombre de fois
     Orientation 1 : 0°
     Orientation 2 : 90°
     Orientation 3 : 180°
     Orientation 4 : 270°
  */
  for(nb; nb > 0; nb--)
  {
    int initiale[5][5];
    initialiser_matrice(initiale);
    affecter_matrice(initiale, c);
    int finale[5][5];
    initialiser_matrice(finale);

    /* Modification des coordonnées des carres de la pièce dans une matrice temporaire */
    for(i = 0; i < 5; i++)
    {
      for(j = 0; j < 5; j++)
      {
        if (initiale[i][j])
        {
          finale[4 - j][i] = 1;
        }
      }
    }

    min_x = 5;
    min_y = 5;

    /* On calcul les coordonnees minimales des carres constituant la pièce */
    for(i = 0; i < 5; i++)
    {
      for(j = 0; j < 5; j++)
      {
        if(finale[i][j])
        {
          if(i < min_x)
            min_x = i;

          if(j < min_y)
            min_y = j;

          c->x = i;
          c->y = j;
          c = carre_get_suiv(c);
        }
      }
    }

    c = piece_liste_carre(p);
    Carre* c2 = c;

    /* On réaffecte les coordonnees minimales des carres constituant la pièce de sorte que x et y >= 0 (On ramène la pièce dans le coin inférieur gauche) */
    do {
      c->x -= min_x;
      c->y -= min_y;
    } while ((c = carre_get_suiv(c)) != c2);
  }
}

int verification_position(Couleur pl[20][20], int x, int y)
{
  return (pl[x][y] == VIDE);
}

int verification_couleur(Couleur pl[20][20], int x, int y, Couleur c)
{
  return ((pl[x-1][y] != c) && (pl[x+1][y] != c) && (pl[x][y-1] != c) && (pl[x][y+1] != c)) && ((pl[x-1][y-1] == c) || (pl[x+1][y-1] == c) || (pl[x-1][y+1] == c) || (pl[x+1][y+1] == c));
}

/* demande au joueur les coordonnees ou il désire jouer sa pièce */
/* Tant qu'il n'est pas possible de jouer aux coords, redemande des coordonnees valides */
void choisir_coordonnee(Couleur pl[20][20], Piece* pi, int* x, int* y, Couleur col)
{
  int placement = 0;
  Carre* c;
  int coord_x, coord_y;
  while(!placement)
  {
    while(!verification_position(pl, coord_x, coord_y))
    {
      c = piece_liste_carre(pi);
      printf("A quelles coordonnees voulez-vous jouer la pièce ? :\n");
      printf("Entrez le x : ");
      scanf("%d", &coord_x);
      printf("Entrez le y : ");
      scanf("%d", &coord_y);
      if(!verification_position(pl, coord_x, coord_y))
        printf("Impossible de placer la piece aux coordonnees indiquees\n");
    }
    c = carre_get_suiv(c);
    while(((c != piece_liste_carre(pi)) && verification_position(pl, coord_x, coord_y) && verification_couleur(pl, coord_x, coord_y, col)))
    {
      c = carre_get_suiv(c);
    }
    if(c == piece_liste_carre(pi))
      placement = 1;
  }
  *x = coord_x;
  *y = coord_y;
}

void poser_piece(Couleur pl[20][20], Piece* pi, Couleur col, int x, int y)
{
  Carre* c = piece_liste_carre(pi);
  while(c != piece_liste_carre(pi))
  {
    pl[x+carre_get_x(c)][y+carre_get_y(c)] = col;
    c = carre_get_suiv(c);
  }
}
















/* */
