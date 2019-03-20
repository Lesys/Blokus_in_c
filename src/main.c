#include "../include/.h"
#include <stdio.h>

int main(){
	char c;
	int choix=0;
	printf("\n Debut du pogramme \n");
	 do{
		printf("\n Entrer 1 pour jouer dans la version terminal, 2 pour la version SDL\n");
                scanf(" %c",&c);

                /* Si l'utilisateur ne rentre pas un entier*/
                if (isdigit(c))
                       choix = atoi(&c);

        } while(!isdigit(c) || choix < 1 || choix > 2);

	if (choix == 1)
		main_terminal();

	else
		main_sdl();

	printf("\n Fin du programme\n");
	return 0;
}
