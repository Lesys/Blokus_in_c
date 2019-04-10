#include "../include/main_sdl.h"
#include "../include/gestion_partie_sdl.h"
#include "../include/sdl.h"

int main_sdl(int fullscreen){

	/* S'il y a eu un problème dans l'ouverture de la SDL */
	if (!sdl_init(fullscreen))
		return 1;

	jouer_partie_sdl();

	sdl_close();

	return 0;
}
