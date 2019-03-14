#ifndef SPRITE_H
#define SPRITE_H
#include <SDL2/SDL.h>

typedef struct sprite {
	SDL_Texture * texture;
	int largeur;
	int longueur;
} Sprite;

Sprite * init_sprite(char * filename, int largeur, int longueur);
void free_sprite(Sprite ** sprite);
void afficher_sprite(Sprite * sprite, int x, int y);

#endif