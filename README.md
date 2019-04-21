# Blokus_in_c

[Documentation du projet](https://lesys.github.io/Blokus_in_c/)

## Compilation du projet

Compilation de la SDL :  
    - Dépendances: libfreetype6 libasound2-dev libpulse-dev libvorbis-dev  
    - Lancer make dans Blokus_in_c/lib/SDL  
Compilation des programmes de test:  
    - make test  
Compilation du projet :  
    - make  
Compilation de la documentation :  
    - doxygen  

## Comment jouer?

Tout d'abord, il faut compiler la SDL (voir plus haut).
Ensuite, il faut de compiler le projet (voir plus haut).
Enfin, il suffit de lancer le programme à l'emplacement "build/blokus.static" pour que le jeu se lance.

=========================================================


## Version 1 (console)

[Fichier conception V1 (console)](https://annuel.framapad.org/p/conduite-projet-l2info-zi-projet)

Implémentation V1:

- Diagramme de gantt

![alt text](docs/IMG/gantt_blokus_version_terminal.png)

## Version 2 (sdl)

[Fichier conception V2 (ajout sdl)](https://annuel.framapad.org/p/conduite-projet-l2info-zi-projet-sdl)

Schémas de l'interface graphique:

- Tour jeu principal

![alt text](docs/IMG/blokus_sdl_conception.png)

- Diagramme de gantt

![alt text](docs/IMG/gantt_blokus_version_sdl.png)

- Interface sous SDL

![alt text](docs/IMG/blokus_jeu_sdl.png)

## Version 3 (réseau & bot)

[Fichier conception V3 (réseau & bot)](https://annuel.framapad.org/p/conduite-projet-l2info-zi-projet-bot-res)

Implémentation V3:

-Diagramme de gantt

![alt text](docs/IMG/gantt_blokus_version_res_bot.png)

