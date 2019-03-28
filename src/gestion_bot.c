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
		return coup->p;
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

Coup* coup_copie(Coup* coup) {
	if (coup == NULL)
		return NULL;

	Coup* copie = malloc(sizeof(*copie));

	copie->p = coup_piece(coup);
	copie->c = coup_couleur(coup);
	copie->x = coup_coord_x(coup);
	copie->y = coup_coord_y(coup);
	copie->valeur_coup = coup_valeur(coup);

	return copie;
}

void coup_afficher(Coup* coup) {
	printf("Couleur: %s\nX: %d, Y: %d\n", couleur_tostring(coup_couleur(coup)), coup_coord_x(coup), coup_coord_y(coup));
	carre_afficher(coup_piece(piece_liste_carre(coup)));
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
        } while(c != piece_liste_carre(coup_piece(piece_liste_carre(coup))));
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

	/* Evalue le nombre de cases disponibles (== VIDE) autour de la nouvelle Piece posée */
	/*eval_cases_dispo(pl2, coup);*/

} /* TODO */

int gestion_tour_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot) {
	int retour = 0;

	Coup* c = bot_jouer_tour(pl, bot);
	coup_afficher(c);
	fprintf(stderr, "Avant psoe piece\n");
    poser_piece_sdl(pl, coup_piece(c), bot, coup_coord_x(c), coup_coord_y(c));
	if (coup_piece(c) == NULL)
		fprintf(stderr, "Ma piece est NULL\n");
free(c);
    Reserves* r = init_afficher_pieces_dispo_sdl(bot);
    SDL_RenderClear(renderer);


	fprintf(stderr, "Avant refresh\n");
    afficher_plateau_sdl(pl);
	fprintf(stderr, "Avant refresh 1\n");
    afficher_pieces_dispo_sdl(r, bot, NULL);
	fprintf(stderr, "Avant refresh 2\n");
    /*afficher_scores_sdl(bot);
	fprintf(stderr, "Avant refresh 3\n");*/
    /*afficher_tour_sdl(bot);
	fprintf(stderr, "Avant refresh 4\n");*/

    SDL_RenderPresent(renderer);
	fprintf(stderr, "Avant refresh 5\n");

    free_afficher_pieces_dispo_sdl(&r);

	fprintf(stderr, "Après refresh\n");

	fprintf(stderr, "Avant affichage\n");
	afficher_plateau(pl);

	fprintf(stderr, "Après pose piece\n");

	return retour;
}

int bot_jouer(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot, int profondeur)
{
    if(profondeur)
    {

    }
}

int adversaire_jouer(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot, Joueur* joueur, int profondeur)
{

}

static void free_tab_coup(Coup*** tab, int taille)
{
    int i;

    if (*tab != NULL) {
        for(i = 0; i < taille; i++)
        {
            free((*tab)[i]);
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

    /* Création d'un petit carre*/
/*    Piece* carre = malloc(sizeof(Piece));
    carre->liste_carre = piece_petit_carre();
    carre->suiv = carre;
    carre->prec = carre;*/

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
                        if (tab == NULL)
                            tab = malloc(sizeof(*tab));
                        else
                            tab = realloc(tab, sizeof(*tab) * (compteur++ + 2));


                        tab[compteur] = malloc(sizeof(*tab[compteur]));

                        tab[compteur]->p = p;

						/* Enlève la Piece actuelle de la liste temporairement */
						/*tab[compteur]->p->prec->suiv = tab[compteur]->p->suiv;*/

						/* Affecte le Coup dans le tableau */
                        tab[compteur]->x = i;
                        tab[compteur]->y = j;

						tab[compteur]->c = joueur_couleur(bot);
tab[compteur]->valeur_coup = 0;
                        /* tab[compteur]->valeur_coup = eval_coup_bot(pl, bot, tab[compteur]->p); */

                        adversaire_jouer(pl, bot, joueur_suivant(bot), PROFONDEUR);

						/* Remet la Piece dans la liste */
						/*tab[compteur]->p->prec->suiv = tab[compteur]->p;*/
						fprintf(stderr, "Piece suivante de p\n");
						carre_afficher(piece_liste_carre(piece_suivant(p)));
						fprintf(stderr, "p\n");
						carre_afficher(piece_liste_carre(p));
						fprintf(stderr, "Piece précédente de p\n");
						carre_afficher(piece_liste_carre(piece_precedent(p)));

						fprintf(stderr, "nb coup possibles %d\n", compteur);
						coup_afficher(tab[compteur]);

						printf("\n");
                    }
                    changer_orientation(p);

					if (!compteur)
						compteur++;
                }
				/*printf("fin de boucle %d\n", tmp);*/
                p = piece_suivant(p);
tmp++;
            } while (p != init);
        }
    }

    Coup* coup = NULL;

	fprintf(stderr, "Avant rand\n");
    if (compteur) {
        nb = rand() % compteur;
			fprintf(stderr, "Avant copie\n");
			coup_afficher(tab[nb]);
        coup = coup_copie(tab[nb]);
			fprintf(stderr, "après copie\n");
			coup_afficher(coup);
    }
	fprintf(stderr, "Après rand\n");

	fprintf(stderr, "Avant free\n");
    free_tab_coup(&tab, compteur);
	fprintf(stderr, "Après Free\n");
		coup_afficher(coup);
			fprintf(stderr, "après affichage\n");

    /* On retourne le coup estimé comme étant le meilleur. NULL si aucun coup n'est possible */
    return coup;
}
