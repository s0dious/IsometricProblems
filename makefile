# Input/Output name variables
executable 	= game

# Flag variables
gl_flags	= -lGL
sfml_flags 	= -lsfml-system -lsfml-window
misc_flags  = -ldl
flags 		= ${gl_flags} ${sfml_flags} ${misc_flags}

# Paths
include		= -I./include
src			= ./src

# Building
all: game

game: src/main.cpp glad camera shaders/*
	g++ src/main.cpp build/*.o -o ${executable} ${flags} ${include}

glad: src/glad.c
	g++ -c src/glad.c -o build/glad.o ${flags} ${include}

camera: src/iso_camera.cpp
	g++ -c src/iso_camera.cpp -o build/camera.o ${flags} ${include}

clean:
	rm -f build/* game