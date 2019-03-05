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

/*
void piece_changer_orientation()
{

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
    for(; nb > 0; nb--)
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
void choisir_coordonnee(Couleur pl[20][20], Piece* pi, int* x, int* y, Joueur* j)
{
    int x_depart;
    int y_depart;
    int placement = 0;
    Carre* c;

    switch(joueur_couleur(j))
    {
        case BLEU:
            x_depart = BLUE_X;
            y_depart = BLUE_Y;
            break;

        case JAUNE:
            x_depart = YELLOW_X;
            y_depart = YELLOW_Y;
            break;

        case ROUGE:
            x_depart = RED_X;
            y_depart = RED_Y;
            break;

        case VERT:
            x_depart = GREEN_X;
            y_depart = GREEN_Y;
            break;

        default:
            break;
    }

    if(joueur_nb_piece_restantes(j) == NB_PIECES)
    {
        int coin = 0;
        Carre* c2;

        while(!coin)
        {
            c = piece_liste_carre(pi);
            c2 = c;

            printf("Vous devez jouer dans votre coin\n");
            printf("A quelles coordonnees voulez-vous jouer la pièce ? :\n");
            printf("Entrez le x : ");
            scanf("%d", x);
            printf("Entrez le y : ");
            scanf("%d", y);

            do
            {
                if((*y + carre_get_y(c) == y_depart) && (*x + carre_get_x(c) == x_depart))
                {
                    coin = 1;
                }

                c = carre_get_suiv(c);
            } while(c != c2);
        }

    }

    /*while(!placement)
    {
        if(joueur_nb_piece_restantes(j) == NB_PIECES)
        {
            int flag = 0;
            int coin = 0;
            do
            {
                c = piece_liste_carre(pi);

                printf("A quelles coordonnees voulez-vous jouer la pièce ? :\n");
                printf("Entrez le x : ");
                scanf("%d", x);
                printf("Entrez le y : ");
                scanf("%d", y);

                c = carre_get_suiv(c);

                while((*y + carre_get_y(c) >= 0) && (*y + carre_get_y(c) < 20) && (*x  + carre_get_x(c) >= 0) && (*x + carre_get_x(c) < 20))
                {
                    c = carre_get_suiv(c);
                    if((*y + carre_get_y(c) == y_depart) && (*x + carre_get_x(c) == x_depart))
                        coin = 1;
                }

                if((*y + carre_get_y(c) > 0) && (*y + carre_get_y(c) < 20) && (*x  + carre_get_x(c) > 0) && (*x + carre_get_x(c) < 20))
                    flag = 1;

                if(coin)
                    printf("C'est votre premier tour, vous devez jouer votre piece dans votre coin\n\n");

            } while(!flag);
        }
        else
        {
            do
            {
                c = piece_liste_carre(pi);

                printf("A quelles coordonnees voulez-vous jouer la pièce ? :\n");
                printf("Entrez le x : ");
                scanf("%d", x);
                printf("Entrez le y : ");
                scanf("%d", y);

                if(!verification_position(pl, *x, *y) || !verification_couleur(pl, *x, *y, joueur_couleur(j)))
                    printf("Impossible de placer la piece aux coordonnees indiquees\n");

            } while(!verification_position(pl, *x, *y) || !verification_couleur(pl, *x, *y, joueur_couleur(j)));
        }

        if(c == piece_liste_carre(pi))
            placement = 1;
    }*/
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
