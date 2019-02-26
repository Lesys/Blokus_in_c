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
  while((nb < 0) && (nb > joueur_nb_piece_restantes(j)))
  {
    printf("Veuillez entrer une valeur correcte\n");
    printf("Quelle pièce voulez-vous jouer ?\n");
    afficher_pieces_dispo(j);
    scanf("%d", &nb);
  }
  for(i = 1; i < nb; i++)
    p = piece_suivant(p);
  return p;
}

void initialiser_matrice(int matrice[5][5])
{
  int i, j;
  for(i = 0; i < 5; i++)
  {
    for(j = 0; j < 5; j++)
      matrice[i][j] = 0;
  }
}

void affecter_matrice(int matrice[5][5], Carre* c)
{
  Carre* c2 = c;
  c2 = carre_get_suiv(c2);
  while(c2 != c)
  {
    matrice[carre_get_x(c2)][carre_get_y(c2)] = 1;
    c2 = carre_get_suiv(c2);
  }
}

/* Affiche les 4 orientations possibles de la pièce au joueur avec un numéro et attend qu'il entre un numéro */
/* modifie également les coordonnees relatives des carres constituant la piece une fois l'orientation choisie */
void demander_orientation(Piece* p)
{
  int nb;
  int max_x = 0;
  int max_y = 0;
  Orientation o;
  Carre* c = piece_liste_carre(p);
  afficher_choix_orientation(p);
  printf("Dans quelle orientation voulez-vous jouer la pièce ? :\n");
  scanf("%d", &nb);
  nb--;
  while((nb < HAUT) || (nb > GAUCHE))
  {
    printf("Veuillez entrer une orientation correcte\n");
    printf("Dans quelle orientation voulez-vous jouer la pièce ? :\n");
    afficher_choix_orientation(p);
    scanf("%d", &nb);
    nb--;
  }
  int sauv, i, j, k, l, m;
  for(nb; nb > 0; nb--)
  {
    int initiale[5][5];
    initialiser_matrice(initiale);
    affecter_matrice(initiale, c);
    int finale[5][5];
    initialiser_matrice(finale);
    for(i = 0; i < 5; i++)
    {
      k = l = m = 0;
      for(j = 0; j < 5; j++)
      {
        if (initiale[i][j])
        {
          l = i;
          m = j;
          sauv = m;
          m = 5 - k;
          l = sauv;
          finale[l][m] = 1;
          k++;
        }
      }
    }
    for(i = 0; i < 5; i++)
    {
      for(j = 0; j < 5; j++)
      {
        if(finale[i][j])
        {
            c->x = i;
            c->y = j;
            c = carre_get_suiv(c);
        }
      }
    }
  }
  /*switch(nb)
  {
    case HAUT:
      break;
    case DROITE:
      sauv = c->x; c->x = c->y; c->y = -(c->x); break;
    case BAS:
      sauv = c->x; c->x = -(c->y); c->y = -(c->x); break;
    case GAUCHE:
      sauv = c->x; c->x = c->y; c->y = c->x; break;
  }
  if(max_x > c->x)
    max_x = c->x;
  if(max_y > c->y)
    max_y = c->y;
  c = carre_get_suiv(c);*/

  /*c = carre_get_suiv(c);
  while(c != piece_liste_carre(p))
  {
    c->x += max_x;
    c->y += max_y;
    c = carre_get_suiv(c);
  }*/
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
