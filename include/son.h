#ifndef SON_H
#define SON_H

#include <SDL2/SDL_mixer.h>

typedef struct ressources_audio {
    /** \brief Son pose pièce */
    Mix_Chunk * pose_piece;
    /** \brief Son cloche */
    Mix_Chunk * cloche;
    /** \brief Son d'abandon */
    Mix_Chunk * abandon;
    /** \brief Son bouton */
    Mix_Chunk * bouton;
    /** \brief Son bouton retour */
    Mix_Chunk * bouton_retour;
    /** \brief Musique de fond */
    Mix_Music * musique_fond;
} Ressources_audio;

typedef enum sons {
    POSE_PIECE,
    CLOCHE,
    ABANDON,
    BOUTON,
    BOUTON_RETOUR,
    MUSIQUE_FOND
} Sons;

int init_son();

void free_son();

void jouer_son(Sons s);

#endif
