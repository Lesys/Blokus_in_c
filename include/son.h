#ifndef SON_H
#define SON_H

#include <SDL2/SDL_mixer.h>

typedef struct ressources_audio {
	Mix_Chunk * pose_piece;
	Mix_Chunk * cloche;
	Mix_Chunk * abandon;
	Mix_Chunk * bouton;
	Mix_Chunk * bouton_retour;
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
