/**
 * \file son.c
 * \brief Fonctions pour jouer des sons avec SDL_Mixer
 * \author BASTIDE Robin
 */

#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/son.h"

// Variables globales de son
Ressources_audio * ressources_audio;

/**
 * \fn int init_son()
 * \brief Initialise le module son
 * \details Fonction à appeler avant tout appel a des
 * fonctions de son.c
 * \return 1 si tout est ok, 0 sinon
 */
int init_son() {

    // Initialisation du struc ressources_audio
    ressources_audio = malloc(sizeof(Ressources_audio));
    if (!ressources_audio) {
        printf("Malloc echoué pour les ressources audio\n");
        return 0;
    }

    if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) {
        free(ressources_audio);
        return 0;
    }

    ressources_audio->pose_piece = Mix_LoadWAV("ressources/pose_piece.wav");
    ressources_audio->cloche = Mix_LoadWAV("ressources/cloche.wav");
    ressources_audio->abandon = Mix_LoadWAV("ressources/abandon.wav");

    return 1;
}

/**
 * \fn void free_son()
 * \brief Quitte le module son
 * \details Fonction à appeler après le dernier
 * appel aux fonctions du fichier son.c
 */
void free_son() {

    if (ressources_audio) {
        Mix_FreeChunk(ressources_audio->pose_piece);
        Mix_FreeChunk(ressources_audio->cloche);
        Mix_FreeChunk(ressources_audio->abandon);
    }

    Mix_CloseAudio();
}

/**
 * \fn void jouer_son(Sons son)
 * \brief Joue un son
 * \details Les différents sont trouvables dans l'énumération Sons
 * \param son Son à jouer
 */
void jouer_son(Sons son) {

    switch (son) {
        case POSE_PIECE:
            Mix_PlayChannel(-1, ressources_audio->pose_piece, 0);
            break;
        case CLOCHE:
            Mix_PlayChannel(-1, ressources_audio->cloche, 0);
            break;
        case ABANDON:
            Mix_PlayChannel(-1, ressources_audio->abandon, 0);
            break;
        default:
            printf("Type de son incorrect\n");
            break;
    }
}