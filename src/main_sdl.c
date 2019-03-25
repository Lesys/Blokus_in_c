#include "../include/main_sdl.h"
#include "../include/gestion_partie_sdl.h"
#include "../include/sdl.h"

int main_sdl(){

	sdl_init();

	jouer_partie_sdl();

	sdl_close();

	return 0;
}
