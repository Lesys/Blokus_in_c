#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/sprite.h"
#include "../include/affichage_sdl.h"
#include "../include/commun.h"
#include "../include/couleur.h"
#include "../include/joueur.h"
#include "../include/sdl.h"

extern SDL_Renderer * renderer;

int main(int arc, char * argv[]) {
    Joueur * lj = joueur_liste_creation(4);

	if (!sdl_init()) {
		printf("Pas d'accord\n");
		return 100;
	}

    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);


    // Initialisation des objets de affichage_sdl
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

    // Ecran a afficher (0 titres, 1 jeu)
    int ecran = 1;
    int running = 1;

    while (running) {
        // Gestion des évenements
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = 0;
                        break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (ecran == 1 && curs_hover_bouton(b_jouer)) {
                    ecran++;
                }
                else if (ecran == 2 && curs_hover_bouton(b_abandonner)) {
                    running = 0;
                }
            }
        }

        // Affichage
        SDL_RenderClear(renderer);

        if (ecran == 1) {
            afficher_titres_sdl();
            afficher_bouton_sdl(b_jouer);
            afficher_bouton_sdl(b_quitter_jeu);
        }
        else if (ecran == 2) {
            afficher_plateau_sdl(plateau);
            afficher_scores_sdl(lj);
            afficher_tour_sdl(lj);
            afficher_bouton_sdl(b_abandonner);
            afficher_resultats_sdl(lj);
            afficher_bouton_sdl(b_continuer);
            afficher_bouton_sdl(b_quitter_partie);
            afficher_pieces_dispo_sdl(r, lj, piece_suivant(piece_suivant(joueur_liste_piece(lj))));
        }

        SDL_RenderPresent(renderer);

    }

    // free des objets de affichage_sdl
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

    // fin affichage_sdl
    free_affichage_sdl();

	// fin sdl
	sdl_close(renderer);
}
