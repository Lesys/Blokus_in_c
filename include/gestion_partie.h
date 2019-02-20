#include "../include/joueur.h"
#include "../include/gestion_tour.h"


void initialisation_partie(joueur * j1); /*Initialisation de la partie, appelle des fonctions pour crée les joueurs, le plateau*/

void jouer_partie();

void jouer_manche(Couleur pl[20][20], Joueur* j);

Joueur* tour_suivant(Joueur* j)

int fin_de_partie(Joueur* j); /* Affiche les résultats, propose les options de fin de partie et renvoie le résultat correspondant */

void maj_scores(Joueur* j, Carre *piece); /* Permet de mettre à jour les scores à la fin de la partie */ 
				/* Si le petit carré a été posé en dernier, ajout des points sur le moment de la pose */

