#Les variables automatiques :
#$@ valeur du  target
#$< nom du premier pré-requis associé au target
#$? nom des pré-requis plus récent que le target
#$^ liste sans répétition des pré-requis
#$+ comme $^ mais avec les répétitions
#$* nom du target avec le suffix tronqué

#Variables pour faciliter la saisie et les modifications dans le Makefile :
#Variable pour le compilateur
CC ?= gcc
GW ?= i586-mingw32msvc-gcc

#Variable où aller chercher les fichiers
VPATH = lib:build:bin:src:include

#Variables pour la confection de la librairie :
#Options pour les lignes de code faites pour la confection des fichiers objets
DIROBJ := bin/
CFLAGS += -L./lib/SDL/lib -I./lib/SDL/include -lSDL2 -lSDL2_ttf -lSDL2_image -fPIC -Wall -std=c99
OBJETS = gestion_tour.o gestion_partie.o joueur.o carre.o affichage.o couleur.o piece.o sprite.o affichage_sdl.o gestion_jeu_sdl.o
FICHIERSC = $(OBJETS:.o=.c)

#Nom du programme principal
DIRMAIN := src/
PROGRPRINC = main.o
PATHMAIN := $(DIRMAIN)$(PROGRPRINC)

#Dossier de  l'exécutable
DIRBUILD := build/

#Noms des librairies statiques et dynamiques
DIRLIB := lib/
LINKNAME ?= Blokus
libSTATIC := lib$(LINKNAME).a
MAJEUR ?= .0
MINEUR ?= .1
CORRECTION ?= .0
SONAMECOURT := lib$(LINKNAME).so
SONAME := $(SONAMECOURT)$(MAJEUR)
REALNAME := $(SONAME)$(MINEUR)$(CORRECTION)

#Variables pour la confection des executables de test :
TESTDIR := $(DIRBUILD)test/
TESTDIRC := src/
TESTOBJETS = test_joueur.o test_carre.o test_affichage.o test_gestion_tour.o test_gestion_partie.o test_affichage_sdl.o test_interactif_affichage_sdl.o test_gestion_jeu_sdl.o

TESTFICHIERSC = $(TESTOBJETS:.o=.c)
TESTEXEC = $(TESTOBJETS:%.o=%)
#TESTSTATIC = test_blokus.static
TESTlibSTATIC := test_lib$(LINKNAME).a
TESTstaticLDLIBS := -l:$(TESTlibSTATIC)

#Options pour les librairies
libCFLAGS = -shared -fPIC -Wl,-soname,
libLDFLAGS = -L.
libLDLIBS = -lc

#Options pour l'archive
ARFLAGS = rcs

#Options des programmes en statique et dynamique
STATIC = blokus.static
DYNAMIC = blokus.shared
EXE = blokus.exe
sharedLDFLAGS := -L.
sharedLDLIBS := -l$(LINKNAME)
staticLDFLAGS := -L.
staticLDLIBS := -l:$(libSTATIC)

#Conception du Makefile :
#Execution du programme en entier
all: | MKDIR MOVE

#Création des dossiers
MKDIR: $(DIRMAIN)$(PROGRPRINC)
	-mkdir $(DIRLIB) $(DIROBJ) $(DIRBUILD)

#Fabrication des fichiers objet .o
$(OBJETS): CFLAGS := $(CFLAGS)
$(OBJETS): $(FICHIERSC)

#Fabrication du fichier objet main.o
$(PROGRPRINC): LDFLAGS := $(CFLAGS)

#Conception de l'archive pour la bibliothèque statique
$(libSTATIC)($(OBJETS)): ARFLAGS := $(ARFLAGS)
$(libSTATIC)($(OBJETS)): $(OBJETS)

#Règle pour appeler la conception de l'archive
$(libSTATIC): $(libSTATIC)($(OBJETS))

#Conception de la bibliothèque dynamique avec ses 2 liens symboliques
$(REALNAME): $(OBJETS)
	$(CC) $(libCFLAGS)$(SONAME) -o $@ $^
	ln -sf $@ $(SONAMECOURT)
	ln -sf $@ $(SONAME)

#Génération du programme static
$(STATIC): LDFLAGS := $(staticLDFLAGS)
$(STATIC): LDLIBS := $(staticLDLIBS)
$(STATIC): $(DIRMAIN)$(PROGRPRINC) $(libSTATIC)
	$(CC) -o $@ $(DIRMAIN)$(PROGRPRINC) $(staticLDFLAGS) $(staticLDLIBS)

#Génération du programme dynamique
$(DYNAMIC): LDFLAGS := $(sharedLDFLAGS)
$(DYNAMIC): LDLIBS := $(sharedLDLIBS)
$(DYNAMIC): $(DIRMAIN)$(PROGRPRINC) $(REALNAME)
	$(CC) -o $@ $(DIRMAIN)$(PROGRPRINC) $(sharedLDFLAGS) $(sharedLDLIBS)

#Execution du programme dynamique
execution: $(DYNAMIC) MOVE clearScreen
	-LD_LIBRARY_PATH=./$(DIRLIB):$LD_LIBRARY_PATH ./$(DIRLIB)$(DYNAMIC)

#Move les fichiers dans leur dossier respectif : .so .a dans le dossier lib. .o dans le dossier bin. L'exécutable dans le dossier build
MOVE: $(DYNAMIC) $(STATIC)
	-mv *.a* *.so* ./$(DIRLIB)
	-mv *blokus* ./$(DIRBUILD)
	-mv *Blokus* ./$(DIRBUILD)
	-mv $(DIRMAIN)*.o $(DIRLIB)*.o *.o ./$(DIROBJ)

#Nettoie les fichiers créés dans le current directory
clean:
	-rm *.o *Blokus* *blokus*
	-rm -R $(DIRBUILD)

#Nettoie les dossiers créés et leur contenu
mrProper: clean clearScreen
	-rm -R $(DIRLIB) $(DIROBJ)

#Nettoie l'écran
clearScreen:
	-clear

#Mets à jour et la push sur le git distant
majDoc:
	-git pull
	-doxygen
	-git add docs/*
	-git commit -m "Maj doc le $(shell date "+%d/%m/%y à %H:%M:%S")"
	-git push origin master


##################### FICHIERS DE TEST #####################

#Lancement des directives pour la création des TEST
test: | TESTMOVE

#Move les fichiers dans leur dossier respectif : .so .a dans le dossier lib. .o dans le dossier bin. Les exécutables de test dans le dossier build
TESTMOVE: $(TESTEXEC)
	-mv $(DIRMAIN)*.o $(DIRLIB)*.o *.o ./$(DIROBJ)
	-mv *test* *Test* ./$(TESTDIR)/

TESTMKDIR:
	-mkdir $(DIRLIB) $(DIROBJ) $(DIRBUILD)
	-mkdir $(TESTDIR)

#Fabrication des executables de TEST
$(TESTOBJETS): CFLAGS := $(CFLAGS)
$(TESTOBJETS): $(TESTFICHIERSC) TESTMKDIR

#Fabrication du fichier objet main.o
#$(TESTOBJETS): LDFLAGS := $(CFLAGS)

#Règle pour appeler la conception de l'archive de TEST
$(TESTlibSTATIC): $(TESTlibSTATIC)($(OBJETS))

#Conception de l'archive pour la bibliothèque statique de TEST
#$(TESTlibSTATIC)($(OBJETS)): ARFLAGS := $(ARFLAGS)
#$(TESTlibSTATIC)($(OBJETS)): $(OBJETS) $(TESTOBJETS)

#Génération des exécutables de TEST
$(TESTEXEC): LDFLAGS := $(staticLDFLAGS)
$(TESTEXEC): LDLIBS := $(staticLDLIBS)
$(TESTEXEC): $(DIRTEST)$(TESTOBJETS) $(libSTATIC)
	$(CC) -o $@ $@.o $(staticLDFLAGS) $(staticLDLIBS) $(CFLAGS)
