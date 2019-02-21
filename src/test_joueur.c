#include <stdio.h>
#include <stdlib.h>
#include "../include/joueur.h"
#include "../include/carre.h"

int main() {
	Joueur* j;

	j = joueur_liste_creation(4);

	printf("Pseudo initial: %s\n", joueur_pseudo(j));

	j = joueur_suivant(j);

	printf("Pseudo suivant: %s\n", joueur_pseudo(j));

	printf("Affichage Piece\n");

	carre_afficher((*joueur_liste_piece(j))->liste_carre);

	printf("Destruiction: \n");
	joueur_liste_detruire(&j);

	if (j == NULL)
		printf("J EST NULL\n");
	else
		printf("J PAS NUL\n");

	printf("Fin destruiction: \n");

	return 0;
}
