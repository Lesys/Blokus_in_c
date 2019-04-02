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
#include <unistd.h>

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

	/* Pose la Piece sur le plateau aux coordonnées saisies */
        do
        {
            pl[x+carre_get_x(c)][y+carre_get_y(c)] = coup_couleur(coup);
            c = carre_get_suiv(c);
        } while(c != piece_liste_carre(coup_piece(coup)));
    }

	return 0;
}

int meilleur_coup(Coup** tab, int compteur) {
	int index_max = 0; /* L'index dans le tableau du meilleur Coup */
	int i = 0;
	int compteur_tab = 0; /* Le nombre de "meilleur coup" */
	int tab_index[compteur]; /* Le tableau contenant les "meilleurs coups" */

	/* Pour tous les Coup */
	for (i = 0; i < compteur; i++)
		/* Si on trouve un Coup avec une meilleure valeur */
		if (coup_valeur(tab[i]) > coup_valeur(tab[index_max])) {

			/* "Réinitialisation" du tableau */
			compteur_tab = 0;
			index_max = i;
		}
		/* Si on trouve un Coup équivalent au meilleur Coup */
		else if (coup_valeur(tab[i]) == coup_valeur(tab[index_max]))
			tab_index[compteur_tab++] = i;


	int random = 0;

	/* S'il y a plus d'un meilleur coup (== compteur_tab n'est pas à 0) */
	if (compteur_tab) {
		srand(time(NULL));
		random = rand() % compteur_tab;
	}
	return tab_index[random];
}

int eval_coup_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup* coup, Joueur* bot) {
	Couleur pl2[TAILLE_PLATEAU][TAILLE_PLATEAU];

	int i, j;
	int eval = 0;

	/* Recopie du plateau */
	for (i = 0; i < TAILLE_PLATEAU; i++)
		for (j = 0; j < TAILLE_PLATEAU; j++)
			pl2[i][j] = pl[i][j];

	poser_piece_bot(pl2, coup);

	/* Evalue le nombre de cases disponibles (== VIDE) autour de la nouvelle Piece posée */
	eval += eval_nb_carres_poses(coup) * COEF_CARRES_POSES;

	if (joueur_nb_piece_restantes(bot) > NB_PIECES - 4)
		eval += eval_emplacement_piece(coup) * COEF_EMPLACEMENT_PIECE;
	else
		eval += (int)(eval_emplacement_piece(coup) * COEF_EMPLACEMENT_PIECE / 2);
	/*eval += eval_cases_dispo(pl2, coup) * COEF_CASES_DISPO;
	eval += eval_nb_nouveaux_coins(pl2, coup) * COEF_NOUVEAUX_COINS;
	eval += eval_nb_coins_bloques(pl2, coup) * COEF_COINS_BLOQUES;
	return eval; */

	return eval;

} /* TODO */

/**
	return Le nombre de Carre de la Piece du Coup joué
*/
int eval_nb_carres_poses(Coup* coup) {
	return piece_nb_carre(coup_piece(coup));
}

int eval_emplacement_piece(Coup* coup) {
	int coord_x = coup_coord_x(coup);
	int coord_y = coup_coord_y(coup);
	int mauvais = 0, moyen = 0, bon = 0, centre = 0; /* Regarde chaque Carre et l'évalue */
	Carre* init = piece_liste_carre(coup_piece(coup));
	Carre* c = init;
	int x = 0, y = 0;

	/* Regarde tous les Carre et leur position */
	do {
		/* Récupère les coordonnées du Carre sur le plateau */
		x = coord_x + carre_get_x(c);
		y = coord_y + carre_get_y(c);

		if (x <= COUP_MAUVAIS || x >= TAILLE_PLATEAU - COUP_MAUVAIS || y <= COUP_MAUVAIS || y >= TAILLE_PLATEAU - COUP_MAUVAIS)
			mauvais++;
		else if (x <= COUP_MOYEN || x >= TAILLE_PLATEAU - COUP_MOYEN || y <= COUP_MOYEN || y >= TAILLE_PLATEAU - COUP_MOYEN)
			moyen++;
		else if (x <= COUP_BON || x >= TAILLE_PLATEAU - COUP_BON || y <= COUP_BON || y >= TAILLE_PLATEAU - COUP_BON)
			bon++;
		else
			centre++;

	} while ((c = carre_get_suiv(c)) != init);

	return (bon * COEF_COUP_BON + moyen * COEF_COUP_MOYEN + mauvais * COEF_COUP_MAUVAIS + centre * COEF_COUP_CENTRE);
}

/**
	return Le nombre de cases dispos autour de la Piece (adjacantes ou en diagonale)
*/
int eval_cases_dispo(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup* coup) {
	int coord_x = coup_coord_x(coup);
	int coord_y = coup_coord_y(coup);

	Carre* init = piece_liste_carre(coup_piece(coup));
	Carre* c = init;
	int x = 0, y = 0;

	/* Regarde tous les Carre et leur position */
	do {
		/* Récupère les coordonnées du Carre sur le plateau */
		x = coord_x + carre_get_x(c);
		y = coord_y + carre_get_y(c);
/*		if (x <= COUP_MAUVAIS || x >= TAILLE_PLATEAU - COUP_MAUVAIS || y <= COUP_MAUVAIS || y >= TAILLE_PLATEAU - COUP_MAUVAIS)
			mauvais++;
		else if (x <= COUP_MOYEN || x >= TAILLE_PLATEAU - COUP_MOYEN || y <= COUP_MOYEN || y >= TAILLE_PLATEAU - COUP_MOYEN)
			moyen++;
		else if (x <= COUP_BON || x >= TAILLE_PLATEAU - COUP_BON || y <= COUP_BON || y >= TAILLE_PLATEAU - COUP_BON)
			bon++;*/

	} while ((c = carre_get_suiv(c)) != init);
}

/**
	return L'ancien nombre de coins libres - le nouveau nombre de coins libres
*/
int eval_nb_nouveaux_coins(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup* coup, int ancien_nb_coup) {
	int coord_x = coup_coord_x(coup);
	int coord_y = coup_coord_y(coup);

	Carre* init = piece_liste_carre(coup_piece(coup));
	Carre* c = init;
	int x = 0, y = 0;

	/* Regarde tous les Carre et leur position */
	do {
		/* Récupère les coordonnées du Carre sur le plateau */
		x = coord_x + carre_get_x(c);
		y = coord_y + carre_get_y(c);
/*		if (x <= COUP_MAUVAIS || x >= TAILLE_PLATEAU - COUP_MAUVAIS || y <= COUP_MAUVAIS || y >= TAILLE_PLATEAU - COUP_MAUVAIS)
			mauvais++;
		else if (x <= COUP_MOYEN || x >= TAILLE_PLATEAU - COUP_MOYEN || y <= COUP_MOYEN || y >= TAILLE_PLATEAU - COUP_MOYEN)
			moyen++;
		else if (x <= COUP_BON || x >= TAILLE_PLATEAU - COUP_BON || y <= COUP_BON || y >= TAILLE_PLATEAU - COUP_BON)
			bon++;*/

	} while ((c = carre_get_suiv(c)) != init);
}

int gestion_tour_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot) {
	int retour = 0;

	Coup* c = bot_jouer_tour(pl, bot);

	/* Si aucun Coup n'a été trouvé */
	if (c == NULL)
		return 1;

	/* Permet de tourner la Piece originale dans la même direction que la copie pour la poser */
	while (!piece_meme_orientation(coup_piece(c), coup_piece_origine(c)))
		piece_pivoter(1, piece_liste_carre(coup_piece_origine(c)));

	/* Pose la Piece et la supprime de la liste du Joueur */
	poser_piece_sdl(pl, coup_piece_origine(c), bot, coup_coord_x(c), coup_coord_y(c));

	/* Suppression du Coup qu'on a joué */
	coup_detruire(&c);

	/* MAJ de l'interface graphique */
    Reserves* r = init_afficher_pieces_dispo_sdl(bot);
    SDL_RenderClear(renderer);

    afficher_plateau_sdl(pl);

    afficher_pieces_dispo_sdl(r, bot, NULL);

    afficher_scores_sdl(bot);

    afficher_tour_sdl(bot);

    SDL_RenderPresent(renderer);

    free_afficher_pieces_dispo_sdl(&r);

	/* sleep(1); */

	return retour;
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

int bot_jouer(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot, int profondeur)
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

						/* Affecte le Coup dans le tableau */
                        tab[compteur]->x = i;
                        tab[compteur]->y = j;

						tab[compteur]->c = joueur_couleur(bot);
						/*tab[compteur]->valeur_coup = 0;*/

						if(!profondeur || tab[compteur]->piece_origine == tab[compteur]->piece_origine->suiv)
							tab[compteur]->valeur_coup = eval_coup_bot(pl, tab[compteur], bot);
						else
						{
							/* Enlève la Piece actuelle de la liste temporairement */
							tab[compteur]->piece_origine->prec->suiv = tab[compteur]->piece_origine->suiv;
							bot->liste_piece = piece_suivant(p);

							/* Recréé un plateau fictif pour émuler les coups */
							Couleur pl2[TAILLE_PLATEAU][TAILLE_PLATEAU];

							int i, j;

							/* Recopie du plateau */
							for (i = 0; i < TAILLE_PLATEAU; i++)
								for (j = 0; j < TAILLE_PLATEAU; j++)
									pl2[i][j] = pl[i][j];

							/* Pose la Piece et NE la supprime PAS de la liste du Joueur */
							poser_piece_bot(pl2, tab[compteur]);

							tab[compteur]->valeur_coup = adversaire_jouer(pl2, bot, joueur_suivant(bot), profondeur - 1);

							/* Remet la Piece dans la liste */
							tab[compteur]->piece_origine->prec->suiv = tab[compteur]->piece_origine;
							bot->liste_piece = p;
						}

						compteur++;
                    }
                    changer_orientation(p);
                }

                p = piece_suivant(p);

            } while (p != init);
        }
    }

	int val_coup = -1;

    if (compteur && tab != NULL) {
		nb = meilleur_coup(tab, compteur);

		val_coup = coup_valeur(tab[nb]);
    }

    free_tab_coup(&tab, compteur);

    /* On retourne le coup estimé comme étant le meilleur. NULL si aucun coup n'est possible */
    return val_coup;
}

int adversaire_jouer(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot, Joueur* joueur, int profondeur)
{
	int i, j, k, nb;

    srand(time(NULL));

    Piece * p = joueur_liste_piece(joueur);
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
                    if(verifier_coordonnees(pl, p, i, j, joueur))
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

						/* Affecte le Coup dans le tableau */
                        tab[compteur]->x = i;
                        tab[compteur]->y = j;

						tab[compteur]->c = joueur_couleur(joueur);
						/*tab[compteur]->valeur_coup = 0;*/

						/* Enlève la Piece actuelle de la liste temporairement */
						tab[compteur]->piece_origine->prec->suiv = tab[compteur]->piece_origine->suiv;
						joueur->liste_piece = piece_suivant(p);

						tab[compteur]->valeur_coup = eval_coup_bot(pl, tab[compteur], joueur);

						/* Remet la Piece dans la liste */
						tab[compteur]->piece_origine->prec->suiv = tab[compteur]->piece_origine;
						joueur->liste_piece = p;

						compteur++;
                    }
                    changer_orientation(p);
                }

                p = piece_suivant(p);

            } while (p != init);
        }
    }

	Coup* coup = NULL;

	int val_coup = -1;

    if (compteur && tab != NULL) {
		nb = meilleur_coup(tab, compteur);

		coup = coup_copie(tab[nb]);

		/* Recréé un plateau fictif pour émuler les coups */
		Couleur pl2[TAILLE_PLATEAU][TAILLE_PLATEAU];

		int i, j;

		/* Recopie du plateau */
		for (i = 0; i < TAILLE_PLATEAU; i++)
			for (j = 0; j < TAILLE_PLATEAU; j++)
				pl2[i][j] = pl[i][j];

		/* Pose la Piece et NE la supprime PAS de la liste du Joueur */
		poser_piece_bot(pl2, coup);

		if((joueur = joueur_suivant(joueur)) == bot)
			val_coup = bot_jouer(pl2, bot, profondeur);
		else
			val_coup = adversaire_jouer(pl2, bot, joueur, profondeur);

		/* Suppression du Coup qu'on a joué */
		coup_detruire(&coup);
    }

    free_tab_coup(&tab, compteur);
	//afficher_pieces_dispo(bot);

    /* On retourne le coup estimé comme étant le meilleur. NULL si aucun coup n'est possible */
    return val_coup;
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
						bot->liste_piece = piece_suivant(p);

						/* Affecte le Coup dans le tableau */
                        tab[compteur]->x = i;
                        tab[compteur]->y = j;

						tab[compteur]->c = joueur_couleur(bot);
						/*tab[compteur]->valeur_coup = 0;*/
                        tab[compteur]->valeur_coup = adversaire_jouer(pl, bot, joueur_suivant(bot), PROFONDEUR);

						/* Remet la Piece dans la liste */
						tab[compteur]->piece_origine->prec->suiv = tab[compteur]->piece_origine;
						bot->liste_piece = p;

						compteur++;
                    }
                    changer_orientation(p);
                }

                p = piece_suivant(p);

            } while (p != init);
        }
    }

    Coup* coup = NULL;

    if (compteur && tab != NULL) {
		nb = meilleur_coup(tab, compteur);

		/*nb = rand() % compteur;*/
        coup = coup_copie(tab[nb]);
    }

    free_tab_coup(&tab, compteur);
	//afficher_pieces_dispo(bot);

    /* On retourne le coup estimé comme étant le meilleur. NULL si aucun coup n'est possible */
    return coup;
}
