#include <stdio.h>
#include <stdlib.h>
#include "../include/piece.h"

int main() {
	Carre* c;
	printf("Creation pieces\n");
	Piece* p = piece_liste_creation();

/*	c = piece_carre();*/

	printf("Affichage pièce\n");
	carre_afficher(p->liste_carre);

	printf("Destruiction: \n");
	liste_piece_detruire(&p);

	if (c == NULL)
		printf("C EST NULL\n");
	else
		printf("C PAS NUL\n");

	printf("Fin destruiction: \n");
	printf("Re destruiction: \n");
	liste_piece_detruire(&p);
	if (c == NULL)
		printf("C EST NULL\n");
	else
		printf("C PAS NUL\n");


	return 0;
}
