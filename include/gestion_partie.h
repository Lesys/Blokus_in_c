#include "commun.h"
#include "joueur.h"
//#include "gestion_tour.h"



void initialisation_partie(Joueur* j); /*Initialisation de la partie, appel des fonctions pour crée les joueurs, le plateau*/

/*Initialisation de la manche*/
void initialisation_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j);

void maj_scores(Joueur* j); /* Permet de mettre à jour les scores à la fin de la partie */ 
/* Si le petit carré a été posé en dernier , ajout des points sur le moment de la pose */

int joueur_abandon(Joueur* j);/*Si tous les joueur on abandonne*/


int fin_de_partie(Joueur* j); /* Affiche les résultats, propose les options de fin de partie et renvoie le résultat correspondant */

Joueur* tour_suivant(Joueur* j);


void jouer_tour(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j);


int jouer_manche(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* j);

void jouer_partie();




