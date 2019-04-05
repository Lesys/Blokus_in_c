#ifndef SPRITE_H
#define SPRITE_H
#include <SDL2/SDL.h>

typedef struct sprite {
    /** \brief Texture du sprite */
    SDL_Texture * texture;
    /** \brief Largeur du sprite */
    int largeur;
    /** \brief Longueur du sprite */
    int longueur;
} Sprite;

Sprite * init_sprite(char * filename, int largeur, int longueur);
void free_sprite(Sprite ** sprite);
void afficher_sprite(Sprite * sprite, int x, int y);

#endif
