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
extern int son;
extern int effet;

/**
 * \fn int init_son()
 * \brief Initialise le module son
 * \details Fonction à appeler avant tout appel a des
 * fonctions de son.c
 * \return 1 si tout est ok, 0 sinon
 */
int init_son() {

    #ifndef SANS_SON
    // Initialisation du struc ressources_audio
    ressources_audio = malloc(sizeof(Ressources_audio));
    if (!ressources_audio) {
        printf("Malloc echoué pour les ressources audio\n");
        return 0;
    }

    if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) {
        printf("Erreur Mix_OpenAudio : %s", Mix_GetError());
        free(ressources_audio);
        return 0;
    }

    ressources_audio->pose_piece = Mix_LoadWAV("ressources/pose_piece.wav");
    ressources_audio->cloche = Mix_LoadWAV("ressources/cloche.wav");
    ressources_audio->abandon = Mix_LoadWAV("ressources/abandon.wav");
    ressources_audio->bouton = Mix_LoadWAV("ressources/bouton.wav");
    ressources_audio->bouton_retour = Mix_LoadWAV("ressources/bouton_retour.wav");
    ressources_audio->musique_fond = Mix_LoadMUS("ressources/musique_fond.ogg");

    #endif
    return 1;
}

/**
 * \fn void free_son()
 * \brief Quitte le module son
 * \details Fonction à appeler après le dernier
 * appel aux fonctions du fichier son.c
 */
void free_son() {

    #ifndef SANS_SON
    if (ressources_audio) {
        Mix_FreeChunk(ressources_audio->pose_piece);
        Mix_FreeChunk(ressources_audio->cloche);
        Mix_FreeChunk(ressources_audio->abandon);
        Mix_FreeChunk(ressources_audio->bouton);
        Mix_FreeChunk(ressources_audio->bouton_retour);
        Mix_FreeMusic(ressources_audio->musique_fond);
    }

    Mix_CloseAudio();
    #endif
}

/**
 * \fn void jouer_son(Sons s)
 * \brief Joue un son
 * \details Les différents sont trouvables dans l'énumération Sons
 * \param s Son à jouer
 */
void jouer_son(Sons s) {

    #ifndef SANS_SON
    switch (s) {
        case POSE_PIECE:
            if (effet) Mix_PlayChannel(-1, ressources_audio->pose_piece, 0);
            break;
        case CLOCHE:
            if (effet) Mix_PlayChannel(-1, ressources_audio->cloche, 0);
            break;
        case ABANDON:
            if (effet) Mix_PlayChannel(-1, ressources_audio->abandon, 0);
            break;
        case BOUTON:
            if (effet) Mix_PlayChannel(-1, ressources_audio->bouton, 0);
            break;
        case BOUTON_RETOUR:
            if (effet) Mix_PlayChannel(-1, ressources_audio->bouton_retour, 0);
            break;
        case MUSIQUE_FOND:
            if (son) Mix_PlayMusic(ressources_audio->musique_fond, -1);
            else Mix_HaltMusic();
            break;
        default:
            printf("Type de son incorrect\n");
            break;
    }
    #endif
}
