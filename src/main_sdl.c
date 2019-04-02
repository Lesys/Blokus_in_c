#include "../include/main_sdl.h"
#include "../include/gestion_partie_sdl.h"
#include "../include/sdl.h"

int main_sdl(int fullscreen){

	sdl_init(fullscreen);

	jouer_partie_sdl();

	sdl_close();

	return 0;
}
