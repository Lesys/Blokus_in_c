.ONESHELL:
all:
	tar xvf SDL2.tar.gz
	cd SDL2-2.0.9
	./configure --prefix=$(shell pwd)
	make
	make install
	cd ..
	rm SDL2-2.0.9
	tar xvf SDL2_image.tar.gz
	cd SDL2_image-2.0.4
	./configure --prefix=$(shell pwd)
	make
	make install
	cd ..
	rm SDL2_image-2.0.4
	tar xvf SDL2_mixer.tar.gz
	cd SDL2_mixer-2.0.4
	./configure --prefix=$(shell pwd)
	make
	make install
	cd ..
	rm SDL2_mixer-2.0.4
	tar xvf SDL2_ttf.tar.gz
	cd SDL2_ttf-2.0.15
	./configure --prefix=$(shell pwd)
	make
	make install
	cd ..
	rm SDL2_ttf-2.0.15
