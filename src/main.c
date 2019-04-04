#include "../include/main_terminal.h"
#include "../include/main_sdl.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(){
	char c;
	int choix=0;
	printf("\n Debut du pogramme \n");
	 do{
		printf("\n Entrez 1 pour jouer dans la version terminal, 2 pour la version SDL, 3 pour la version SDL plein ecran\n");
                scanf(" %c",&c);

                /* Si l'utilisateur ne rentre pas un entier*/
                if (isdigit(c))
                       choix = atoi(&c);

        } while(!isdigit(c) || choix < 1 || choix > 3);

	if (choix == 1)
		main_terminal();

	else if (choix == 2)
		main_sdl(0);

	else
		main_sdl(1);

	printf("\n Fin du programme\n");
	return 0;
}
