#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/sprite.h"
#include "../include/affichage_sdl.h"
#include "../include/commun.h"
#include "../include/couleur.h"
#include "../include/joueur.h"

SDL_Window * window;
SDL_Renderer * renderer;

int main(int arc, char * argv[]) {
    Joueur * lj = joueur_liste_creation(4);
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("test_affichage_sdl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, L_FENETRE, H_FENETRE, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("%s", SDL_GetError());
        return 0;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("%s", SDL_GetError());
        return 0;
    }

    init_affichage_sdl();

    Couleur plateau[TAILLE_PLATEAU][TAILLE_PLATEAU] = {0};
    Reserves * r = init_afficher_pieces_dispo_sdl(lj);
    Bouton * b_abandonner = init_bouton_sdl(ABANDONNER);
    Bouton * b_continuer = init_bouton_sdl(CONTINUER);
    Bouton * b_quitter_partie = init_bouton_sdl(QUITTER_PARTIE);
    Bouton * b_jouer = init_bouton_sdl(JOUER);
    Bouton * b_quitter_jeu = init_bouton_sdl(QUITTER_JEU);
    Bouton * b_nb_joueurs_2 = init_bouton_sdl(NB_JOUEURS_2);
    Bouton * b_nb_joueurs_3 = init_bouton_sdl(NB_JOUEURS_3);
    Bouton * b_nb_joueurs_4 = init_bouton_sdl(NB_JOUEURS_4);
    Bouton * b_type_joueur_local = init_bouton_sdl(TYPE_JOUEUR_LOCAL);
    Bouton * b_type_joueur_distant = init_bouton_sdl(TYPE_JOUEUR_DISTANT);
    Bouton * b_type_joueur_bot = init_bouton_sdl(TYPE_JOUEUR_BOT);

    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);

    SDL_RenderClear(renderer);

    /*afficher_plateau_sdl(plateau);
      afficher_pieces_dispo_sdl(r, lj, NULL);
      afficher_scores_sdl(lj);
      afficher_tour_sdl(lj);
      afficher_bouton_sdl(b_abandonner);
      afficher_resultats_sdl(lj);
      afficher_bouton_sdl(b_continuer);
      afficher_bouton_sdl(b_quitter_partie);
      */

    /*
       afficher_titres_sdl();
       afficher_bouton_sdl(b_jouer);
       afficher_bouton_sdl(b_quitter_jeu);
       */


    afficher_nb_joueurs_sdl();
    afficher_bouton_sdl(b_nb_joueurs_2);
    afficher_bouton_sdl(b_nb_joueurs_3);
    afficher_bouton_sdl(b_nb_joueurs_4);

    /*
       afficher_type_joueur_sdl();
       afficher_bouton_sdl(b_type_joueur_local);
       afficher_bouton_sdl(b_type_joueur_distant);
       afficher_bouton_sdl(b_type_joueur_bot);
       */

    //afficher_saisie_pseudo_sdl("Pseudotaillemax");

    SDL_RenderPresent(renderer);

    sleep(10);


    free_afficher_pieces_dispo_sdl(&r);

    free_bouton_sdl(&b_abandonner);
    free_bouton_sdl(&b_continuer);
    free_bouton_sdl(&b_quitter_partie);
    free_bouton_sdl(&b_jouer);
    free_bouton_sdl(&b_quitter_jeu);
    free_bouton_sdl(&b_nb_joueurs_2);
    free_bouton_sdl(&b_nb_joueurs_3);
    free_bouton_sdl(&b_nb_joueurs_4);
    free_bouton_sdl(&b_type_joueur_local);
    free_bouton_sdl(&b_type_joueur_distant);
    free_bouton_sdl(&b_type_joueur_bot);
    free_affichage_sdl();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
