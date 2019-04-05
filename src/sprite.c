/**
 * \file sprite.c
 * \brief Fonctions de gestions des images pour la SDL
 * \details Diverses fonctions qui gèrent le chargement l'affiche
 * et la désallocation des images
 * \author BASTIDE Robin
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/sprite.h"

// Variables globales externes
extern SDL_Renderer * renderer;

/**
 * \fn static SDL_Texture* init_texture(char * filename)
 * \brief Initialisation d'une texture SDL
 * \param filename Nom du fichier à ouvrir
 * \return La texture au format sdl, NULL si erreur
 */
static
SDL_Texture* init_texture(char * filename) {

    // Chargement de l'image dans une surface
    SDL_Surface * image = IMG_Load(filename);
    if (!image) {

        printf("Impossible de charger %s : %s\n", filename, IMG_GetError());
        return NULL;

    }

    // Création de la texture depuis la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    if (!texture) {

        printf("Impossible de créer une texture à partir du fichier %s : %s\n", filename, SDL_GetError());
        return NULL;

    }

    // Désallocation de la surface
    SDL_FreeSurface(image);

    return texture;
}

/**
 * \fn Sprite * init_sprite(char * filename, int longueur, int largeur)
 * \brief Initialisation d'un sprite
 * \param filename Nom du fichier à ouvrir
 * \param longueur Longueur (hauteur) du sprite à créer
 * \param largeur Largeur du sprite à créer
 * \return Le sprite initialisé, NULL si erreur
 */
Sprite * init_sprite(char * filename, int longueur, int largeur) {

    // Allocation de la structure
    Sprite * sprite = malloc(sizeof(Sprite));
    if (!sprite) {
        printf("malloc a échoué dans init_sprite()\n");
        return NULL;
    }
    else {
        // Création est mise en place de la texture
        sprite->texture = init_texture(filename);
        if (!sprite->texture) {
            free(sprite);
            return NULL;
        }
        else {
            // Mise en place de la largeur et longueur
            sprite->largeur = largeur;
            sprite->longueur = longueur;

            return sprite;
        }
    }
}

/**
 * \fn void free_sprite(Sprite ** sprite)
 * \brief Désallocation d'un sprite
 * \param sprite Sprite à supprimer
 */
void free_sprite(Sprite ** sprite) {

    if (*sprite) {
        SDL_DestroyTexture((*sprite)->texture);
        free(*sprite);
        *sprite = NULL;
    }
}

/**
 * \fn void afficher_sprite(Sprite * sprite, int x, int y)
 * \brief Affichage d'un sprite
 * \param sprite Sprite à afficher
 * \param x Coordonnée en x du sprite
 * \param y Coordonnée en y du sprite
 */
void afficher_sprite(Sprite * sprite, int x, int y) {

    // Création du rectangle de destination
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = sprite->largeur;
    dstrect.h = sprite->longueur;

    // Affichage
    SDL_RenderCopy(renderer, sprite->texture, NULL, &dstrect);
}
