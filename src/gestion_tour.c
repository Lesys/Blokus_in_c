#include "../include/gestion_tour.h"
#include "../include/affichage.h"
#include "../include/carre.h"

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

    /* Tant que le joueur entre un numéro ne correspondant pas à une pièce disponible, on redemande un numéro valide */
    while((nb <= 0) && (nb > joueur_nb_piece_restantes(j)))
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
void demander_orientation(Piece* p, Joueur* j)
{
    int nb;
    Carre* c = piece_liste_carre(p);

    afficher_choix_orientation(p, j);
    printf("Dans quelle orientation voulez-vous jouer la pièce ? :\n");
    scanf("%d", &nb);
    nb--;

    /* tant que le joueur n'entre pas un numéro d'orientation correct, on lui redemande un numéro valide */
    while((nb < HAUT) || (nb > GAUCHE))
    {
        printf("Veuillez entrer une orientation correcte\n");
        printf("Dans quelle orientation voulez-vous jouer la pièce ? :\n");
        afficher_choix_orientation(p, j);
        scanf("%d", &nb);
        nb--;
    }
    piece_pivoter(nb, c);
}

int verification_position(Couleur pl[20][20], int x, int y, Piece* p)
{
    Carre* c = piece_liste_carre(p);

    do
    {
        if(pl[x+carre_get_x(c)][y+carre_get_y(c)] != VIDE)
        {
            return 0;
        }
        c = carre_get_suiv(c);
    } while(c != piece_liste_carre(p));
    return 1;
}

/* Fonction qui vérifie si aucun Carre de la Couleur du Joueur n'est adjacant aux Carre que le Joueur veut poser
	et qu'il y a au moins un Carre de la Couleur du Joueur qui est en diagonale d'un Carre que le Joueur veut poser */
int verification_couleur(Couleur pl[20][20], int x, int y, Couleur col, Piece* p)
{
    Carre* c = piece_liste_carre(p);
    int angle = 0;

    do
    {
	/* Vérifie qu'il n'y a aucun Carre adjacant aux Carre que le Joueur pose */
        if(pl[x + carre_get_x(c) - 1][y + carre_get_y(c)] == col || /* Au dessus */
		pl[x + carre_get_x(c) + 1][y + carre_get_y(c)] == col || /* En dessous */
		pl[x + carre_get_x(c)][y + carre_get_y(c) - 1] == col || /* A gauche */
		pl[x + carre_get_x(c)][y + carre_get_y(c) + 1] == col) /* A droite */
        {
            return 0;
        }

	/* Vérifie qu'il y a au moins un Carre que le Joueur pose qui touche diagonalement un Carre déjà posé de même Couleur */
        if((pl[x + carre_get_x(c) - 1][y + carre_get_y(c) - 1] == col) || /* Diagonale Haut - Gauche */
		(pl[x + carre_get_x(c) + 1][y + carre_get_y(c) - 1] == col) || /* Diagonale Bas - Gauche */
		(pl[x + carre_get_x(c) - 1][y + carre_get_y(c) + 1] == col) || /* Diagonale Haut - Droit */
		(pl[x + carre_get_x(c) + 1][y + carre_get_y(c) + 1] == col)) /* Diagonale Bas - Droit */
        {
            angle = 1;
        }

        c = carre_get_suiv(c);

    } while(c != piece_liste_carre(p));

    /* Si au moins un Carre est en diagonale d'un Carre de même Couleur déjà posé */
    if(angle)
    {
        return 1;
    }

    /* Si aucun return n'a été fait précédemment */
    return 0;
    /* return ((pl[x-1][y] != c) && (pl[x+1][y] != c) && (pl[x][y-1] != c) && (pl[x][y+1] != c)) && ((pl[x-1][y-1] == c) || (pl[x+1][y-1] == c) || (pl[x-1][y+1] == c) || (pl[x+1][y+1] == c)); */
}

/* Demande au joueur les coordonnees ou il désire jouer sa pièce */
/* Tant qu'il n'est pas possible de jouer aux coords, redemande des coordonnees valides */
void choisir_coordonnee(Couleur pl[20][20], Piece* pi, int* x, int* y, Joueur* j)
{
    int x_depart;
    int y_depart;
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

    /* Si la Piece que le Joueur pose est sa toute première Piece, doit jouer dans son coin */
    if(joueur_nb_piece_restantes(j) == NB_PIECES)
    {
        int coin = 0;
        Carre* c2;

        while(!coin)
        {
            c = piece_liste_carre(pi);

            int valide;

	    /* Tant que les coordonnées saisies ne sont pas dans le Plateau */
            do
            {
	        c2 = piece_liste_carre(pi);

                valide = 1;

		printf("Vous devez jouer dans votre coin\n");
		printf("A quelles coordonnees voulez-vous jouer la pièce ? (1 a 20) :\n");
		printf("Entrez la ligne : ");
		scanf("%d", x);
		printf("Entrez la colonne : ");
		scanf("%d", y);
		*x = *x - 1;
		*y = *y - 1;

                do
                {
		    /* Si un Carre n'est pas dans le plateau */
		    if(((*x + carre_get_x(c) < 0) || (*x + carre_get_x(c) > 19)) || ((*y + carre_get_y(c) < 0) || (*y + carre_get_y(c) > 19)))
		    {
                    	valide = 0;
		    }

		    c = carre_get_suiv(c);

                } while(c != c2);

	    } while((((*x < 0) || (*x > 19)) || ((*y < 0) || (*y > 19))) || !(valide));

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
    else
    {
        /* A FAIRE
        do
        {*/
            c = piece_liste_carre(pi);
            Carre *c2 = c;
            carre_get_suiv(c);

            int dans_plateau = 1;

            do
            {
        				printf("A quelles coordonnees voulez-vous jouer la pièce ? :\n");
        				printf("Entrez le x : ");
        				scanf("%d", x);
        				printf("Entrez le y : ");
        				scanf("%d", y);
       					*x = *x - 1;
                *y = *y - 1;
                while(c != c2)
                {
                  if(((*x < 0) || (*x > 19)) || ((*y < 0) || (*y > 19)))
                  {
                    dans_plateau = 0;
                  }
                }
            } while(!dans_plateau);

            if(!verification_position(pl, *x, *y, pi) || !verification_couleur(pl, *x, *y, joueur_couleur(j), pi))
            {
                printf("Impossible de placer la piece aux coordonnees indiquees\n");
                *x = *y = -1;
            }

        /*} while(!verification_position(pl, *x, *y) || !verification_couleur(pl, *x, *y, joueur_couleur(j)));*/
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

void poser_piece(Couleur pl[20][20], Piece* pi, Joueur* j, int x, int y)
{
    Carre* c = piece_liste_carre(pi);
    Piece** p = &(j->liste_piece);
    Piece* pivot = *p;
    /*Piece* p = piece_suivant(joueur_liste_piece(j));*/

    do
    {
        pl[x+carre_get_x(c)][y+carre_get_y(c)] = joueur_couleur(j);
        c = carre_get_suiv(c);
    } while(c != piece_liste_carre(pi));

    while ((*p) != pi)
        *p = piece_suivant(*p);

	if (pivot == *p)
		pivot = NULL;

    liste_piece_suppr_elem(p);

    while (pivot != NULL && (*p) != pivot)
        *p = piece_suivant(*p);

    j->liste_piece = *p;
}
















/* */
