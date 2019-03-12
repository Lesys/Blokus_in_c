.ONESHELL:
all: clean compilSDL

clean:
	rm include -r
	rm lib -r
	rm share -r
	rm bin -r
	rm SDL2-2.0.9 -r
	rm SDL2_image-2.0.4 -r
	rm SDL2_mixer-2.0.4 -r
	rm SDL2_ttf-2.0.15 -r

compilSDL:
	tar xvf SDL2.tar.gz
	cd SDL2-2.0.9
	./configure --prefix=$(shell pwd)
	make
	make install
	cd ..
	rm SDL2-2.0.9 -r
	tar xvf SDL2_image.tar.gz
	cd SDL2_image-2.0.4
	./configure --prefix=$(shell pwd)
	make
	make install
	cd ..
	rm SDL2_image-2.0.4 -r
	tar xvf SDL2_mixer.tar.gz
	cd SDL2_mixer-2.0.4
	./configure --prefix=$(shell pwd)
	make
	make install
	cd ..
	rm SDL2_mixer-2.0.4 -r
	tar xvf SDL2_ttf.tar.gz
	cd SDL2_ttf-2.0.15
	./configure --prefix=$(shell pwd)
	make
	make install
	cd ..
	rm SDL2_ttf-2.0.15 -r
