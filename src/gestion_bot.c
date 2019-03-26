#include "../include/gestion_tour_bot.h"
#include "../include/gestion_tour.h"
#include "../include/couleur.h"
#include "../include/joueur.h"

#include <time.h>

int eval_coup_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Piece* p) {} /* TODO */

int gestion_tour_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot) {
	Piece* p = bot_jouer_tour(pl, bot)
        poser_piece_sdl(pl, *p, bot, x, y);
}
