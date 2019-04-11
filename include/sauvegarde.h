#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <stdio.h>
#include <string.h>

#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/couleur.h"

int sauvegarder_partie(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur*, char*);

int charger_partie(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur*, char*);

#endif
