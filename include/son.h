#ifndef SON_H
#define SON_H

#include <SDL2/SDL_mixer.h>

typedef struct ressources_audio {
	Mix_Chunk * pose_piece;
	Mix_Chunk * cloche;
	Mix_Chunk * abandon;
} Ressources_audio;

typedef enum sons {
	POSE_PIECE,
	CLOCHE,
	ABANDON
} Sons;

int init_son();

void free_son();

void jouer_son(Sons son);

#endif
