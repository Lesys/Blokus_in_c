#include "../include/couleur.h"

char* couleur_tostring(Couleur c) {
	switch (c) {
		case BLEU: return "Bleu";
			break;
		case JAUNE: return "Jaune";
			break;
		case VERT: return "Vert";
			break;
		case ROUGE: return "Rouge";
			break;
		case VIDE: return "Vide";
			break;
		default: return "Aucune couleur";
			break;
	}
}

