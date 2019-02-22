#ifndef COULEUR_H
#define COULEUR_H

typedef enum couleur {
    VIDE,
    BLEU,
    JAUNE,
    ROUGE,
    VERT
} Couleur;

/* Renvoie la couleur en chaine de caractère */
char* couleur_tostring(Couleur);

#endif
