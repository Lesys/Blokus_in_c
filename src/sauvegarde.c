
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/carre.h"
#include "../include/couleur.h"

/**
	\fn int sauvegarder_partie(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* joueur, char* filename);
	\brief Sauvegarde la partie dans un fichier f

	\param pl Le plateau de la parite à sauvegarder
	\param joueur la liste des joueurs avec en premier, le joueur devant jouer au moment de la sauvegarde
    \param filename nom du fichier dans lequel sera sauvegardée la partie
	\return code d'erreur : 0 si le fichier a été ouvert, 1 si le fichier n'a pas pu être ouvert
*/
int sauvegarder_partie(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* joueur, char* filename)
{
    struct stat st = {0};

    if (stat("./save", &st) == -1)
        mkdir("./save", 0700);
    FILE* f;

    /* Création de la chaine de caractère dossier/fichier.extension */
    char nom_fichier[TAILLE_NOM_FICHIER + 4];
    strcpy(nom_fichier, strcat(filename, EXTENSION_SAVE_FICHIER));

    char nom_dossier[TAILLE_NOM_FICHIER + strlen(DOSSIER_SAVE_FICHIER) + strlen(EXTENSION_SAVE_FICHIER)];
    strcpy(nom_dossier, DOSSIER_SAVE_FICHIER);

    strcat(nom_dossier, nom_fichier);
    f = fopen(nom_dossier, "w");

    if(f)
    {
        int i, j;

        /* Sauvegarde du plateau dans le fichier f */
        for(i = 0; i < TAILLE_PLATEAU; i++)
        {
            for(j = 0; j < TAILLE_PLATEAU; j++)
                fprintf(f, "%d ", pl[i][j]);

            fprintf(f, "\n");
        }

        /* Sauvegarde des joueurs et de leurs pièces restantes dans le fichier f */
        Joueur* joueur2 = joueur;

        do
        {
            /* Sauvegarde des informations des joueurs */
            fprintf(f, "%d %s %d %d %d %d ", (int) joueur_couleur(joueur), joueur_pseudo(joueur), (int) joueur_type_joueur(joueur), joueur_a_abandonne(joueur), joueur_score(joueur), joueur_sockfd(joueur));

            Piece* piece = joueur_liste_piece(joueur);
            Piece* piece2 = piece;

            /* Sauvegarde des id des pièces restantes */
            if(!piece_hors_liste(piece))
            {
                do
                {
                    fprintf(f, "%d ", piece_id(piece));

                } while((piece = piece_suivant(piece)) != piece2);
            }

            fprintf(f, "0\n", piece_id(piece));

        } while((joueur = joueur_suivant(joueur)) != joueur2);

    }
    else
    {
        return 1;
    }

    fclose(f);

    return 0;
}

int charger_partie(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* joueur, char* filename)
{
    /* Création de la chaine de caractère dossier/fichier.extension */
    char nom_fichier[TAILLE_NOM_FICHIER + 4];
    strcpy(nom_fichier, strcat(filename, EXTENSION_SAVE_FICHIER));

    char nom_dossier[TAILLE_NOM_FICHIER + strlen(DOSSIER_SAVE_FICHIER) + strlen(EXTENSION_SAVE_FICHIER)];
    strcpy(nom_dossier, DOSSIER_SAVE_FICHIER);

    strcat(nom_dossier, nom_fichier);

    FILE* f;

    f = fopen(strcat(DOSSIER_SAVE_FICHIER, strcat(filename, EXTENSION_SAVE_FICHIER)), "r");

    if(f)
    {
        int i, j;

        /* Chargement du plateau depuis le fichier f */
        for(i = 0; i < TAILLE_PLATEAU; i++)
        {
            for(j = 0; j < TAILLE_PLATEAU; j++)
                fscanf(f, "%d", &(pl[i][j]));
        }

        Joueur* joueur2 = NULL;
        Couleur c;
        char nom [TAILLE_PSEUDO];
        int type, abandon, score, sockfd, id;
        int tab[NB_PIECES];
        int cpt;

        /* Chargement des joueurs et de leurs pièces depuis le fichier f */
        while(fscanf(f, "%d %s %d %d %d %d ", &c, nom, &type, &abandon, &score, &sockfd))
        {
            cpt = 0;

            /* Si ce n'est pas le premier joueur */
            if(joueur2 != NULL)
                joueur2 = joueur2->suiv;

            joueur2 = malloc(sizeof(Joueur));

            /* Chargement des informations des joueurs */
            joueur2->couleur = c;
            joueur2->pseudo = nom;
            joueur2->type = type;
            joueur2->abandon = abandon;
            joueur2->score = score;
            joueur2->sockfd = sockfd;

            joueur2->liste_piece = piece_liste_creation();

            /* Récupère les id des pièces restantes pour le joueur sauvegardées dans le fichier f */
            while (fscanf(f, "%d", id) && id != 0)
                tab[cpt++] = id;

            liste_piece_charger(tab, cpt, joueur2->liste_piece);

            joueur2->suiv = NULL;
        }

        /* Permet de terminer la boucle en faisant pointer le joueur suivant du dernier joueur sur le premier */
        joueur2->suiv = joueur;
    }
    else
    {
        return 1;
    }

    fclose(f);

    return 0;
}





















/* */
