#include "../include/gestion_bot.h"
#include "../include/gestion_tour.h"
#include "../include/gestion_tour_sdl.h"
#include "../include/couleur.h"
#include "../include/joueur.h"
#include "../include/commun.h"
#include "../include/affichage_sdl.h"
#include "../include/affichage.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <SDL2/SDL.h>

extern SDL_Renderer* renderer;

/* Accesseurs */
Piece* coup_piece (Coup* coup) {
	if (coup != NULL)
		return coup->piece_copie;
	return NULL;
}

Piece* coup_piece_origine (Coup* coup) {
	if (coup != NULL)
		return coup->piece_origine;
	return NULL;
}

Couleur coup_couleur (Coup* coup) {
	return coup->c;
}

int coup_coord_x (Coup* coup) {
	return coup->x;
}

int coup_coord_y (Coup* coup) {
	return coup->y;
}

int coup_valeur (Coup* coup) {
	return coup->valeur_coup;
}

void coup_detruire(Coup** coup) {
	if ((*coup) != NULL) {
		if ((*coup)->piece_copie != NULL)
			liste_piece_detruire(&((*coup)->piece_copie));

		free(*coup);
	}

	*coup = NULL;
}

Coup* coup_copie(Coup* coup) {
	if (coup == NULL)
		return NULL;

	Coup* copie = malloc(sizeof(*copie));

	/* Copie la Piece (car elle peut être modifiée plus tard. On ne veut pas) */
	copie->piece_copie = piece_copie(coup_piece(coup));

	/* Enregistrement de son adresse pour la supprimer si on la pose */
	copie->piece_origine = coup_piece_origine(coup);
	copie->c = coup_couleur(coup);
	copie->x = coup_coord_x(coup);
	copie->y = coup_coord_y(coup);
	copie->valeur_coup = coup_valeur(coup);

	return copie;
}

void coup_afficher(Coup* coup) {
	printf("Couleur: %s\nX: %d, Y: %d\n", couleur_tostring(coup_couleur(coup)), coup_coord_x(coup), coup_coord_y(coup));

	printf("Piece copie\n");
	carre_afficher(piece_liste_carre(coup_piece(coup)));
	printf("Piece origine\n");
	carre_afficher(piece_liste_carre(coup_piece_origine(coup)));
}

static int poser_piece_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup* coup) {
    if(!piece_hors_liste(coup_piece(coup)))
    {
	int x = coup_coord_x(coup);
	int y = coup_coord_y(coup);

        Carre* c = piece_liste_carre(coup_piece(coup));

        do
        {
            pl[x+carre_get_x(c)][y+carre_get_y(c)] = coup_couleur(coup);
            c = carre_get_suiv(c);
        } while(c != piece_liste_carre(coup_piece(coup)));
    }

	return 0;
}

int eval_coup_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup* coup) {
	Couleur pl2[TAILLE_PLATEAU][TAILLE_PLATEAU];


	int i, j;
	for (i = 0; i < TAILLE_PLATEAU; i++)
		for (j = 0; j < TAILLE_PLATEAU; j++)
			pl2[i][j] = pl[i][j];

	poser_piece_bot(pl2, coup);

	return 1;
	/* Evalue le nombre de cases disponibles (== VIDE) autour de la nouvelle Piece posée */
	/*eval_cases_dispo(pl2, coup);*/

} /* TODO */

int gestion_tour_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot) {
	int retour = 0;

	Coup* c = bot_jouer_tour(pl, bot);

	/* Si aucun Coup n'a été trouvé */
	if (c == NULL)
		return 1;

	while (!piece_meme_orientation(coup_piece(c), coup_piece_origine(c)))
		piece_pivoter(1, piece_liste_carre(coup_piece_origine(c)));

	poser_piece_sdl(pl, coup_piece_origine(c), bot, coup_coord_x(c), coup_coord_y(c));

	coup_detruire(&c);

    Reserves* r = init_afficher_pieces_dispo_sdl(bot);
    SDL_RenderClear(renderer);


    afficher_plateau_sdl(pl);

    afficher_pieces_dispo_sdl(r, bot, NULL);

    afficher_scores_sdl(bot);

    afficher_tour_sdl(bot);

    SDL_RenderPresent(renderer);

    free_afficher_pieces_dispo_sdl(&r);

	return retour;
}

int bot_jouer(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot, int profondeur)
{
    if(profondeur)
    {

    }

	return 1;
}

int adversaire_jouer(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot, Joueur* joueur, int profondeur)
{
	return -1;
}

static void free_tab_coup(Coup*** tab, int taille)
{
    int i;

    if (*tab != NULL) {
        for(i = taille - 1; i >= 0; i--)
	{
            coup_detruire(&((*tab)[i]));
        }

        free(*tab);
    }

    *tab = NULL;
}

/* Gestion du premier tour du bot */
Coup* bot_jouer_tour(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot)
{
    int i, j, k, nb;

    srand(time(NULL));

    Piece * p = joueur_liste_piece(bot);
    Piece * init = p;

    Coup** tab = NULL;
    int compteur = 0, tmp = 0;

    /* Pour chaque position de la matrice */
    for(i = 0; i < TAILLE_PLATEAU; i++)
    {
        for(j = 0; j < TAILLE_PLATEAU; j++)
        {
            /* Pour chaque pièces disponibles */
            do
            {
                /* Pour chaque orientation possible */
                for(k = 0; k < 4; k++)
                {
                    /* Si la pièce est posable */
                    if(verifier_coordonnees(pl, p, i, j, bot))
                    {
                        /* On enregistre le coup puis on estime sa valeur */
                        if (tab == NULL) {
                            tab = malloc(sizeof(*tab));
			}
                        else
                            tab = realloc(tab, sizeof(*tab) * (compteur + 1));


                        tab[compteur] = malloc(sizeof(*tab[compteur]));

                        tab[compteur]->piece_origine = p;

                        tab[compteur]->piece_copie = piece_copie(p);

			/* Enlève la Piece actuelle de la liste temporairement */
			tab[compteur]->piece_origine->prec->suiv = tab[compteur]->piece_origine->suiv;

			/* Affecte le Coup dans le tableau */
                        tab[compteur]->x = i;
                        tab[compteur]->y = j;

			tab[compteur]->c = joueur_couleur(bot);
			tab[compteur]->valeur_coup = 0;
                        /* tab[compteur]->valeur_coup = eval_coup_bot(pl, bot, tab[compteur]->p); */

                        adversaire_jouer(pl, bot, joueur_suivant(bot), PROFONDEUR);

			/* Remet la Piece dans la liste */
			tab[compteur]->piece_origine->prec->suiv = tab[compteur]->piece_origine;

			compteur++;
                    }
                    changer_orientation(p);
                }

                p = piece_suivant(p);

            } while (p != init);
        }
    }

    Coup* coup = NULL;

    if (compteur) {
        nb = rand() % compteur;
        coup = coup_copie(tab[nb]);
    }

    free_tab_coup(&tab, compteur);
//	afficher_pieces_dispo(bot);

    /* On retourne le coup estimé comme étant le meilleur. NULL si aucun coup n'est possible */
    return coup;
}
