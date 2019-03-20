#include <stdio.h>
#include <stdlib.h>

#include "../include/gestion_partie_sdl.h"
#include "../include/sdl.h"

int main(){

	sdl_init();

	jouer_partie_sdl();

	sdl_close();
	return 0;
}
