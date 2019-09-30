# Input/Output name variables
executable 	= game

# Flag variables
cpp_flags	= -std=c++11 -Wall -Werror
gl_flags	= -lGL
sfml_flags 	= -lsfml-system -lsfml-window
misc_flags  = -ldl
flags 		= ${gl_flags} ${sfml_flags} ${misc_flags} ${cpp_flags}

iso_modules = camera character database input map model shader

# Paths
include		= -I./include
src			= ./src

# Building
all: game

game: src/main.cpp glad ${iso_modules} shaders/*
	g++ src/main.cpp build/*.o -o ${executable} ${flags} ${include}

glad: src/glad.c
	g++ -c src/glad.c -o build/glad.o ${flags} ${include}

# Project code

camera: src/iso_camera.cpp
	g++ -c src/iso_camera.cpp -o build/camera.o ${flags} ${include}

character: src/iso_character.cpp
	g++ -c src/iso_character.cpp -o build/character.o ${flags} ${include}

database: src/iso_database.cpp
	g++ -c src/iso_database.cpp -o build/database.o ${flags} ${include}

input: src/iso_input.cpp
	g++ -c src/iso_input.cpp -o build/input.o ${flags} ${include}

map: src/iso_map.cpp
	g++ -c src/iso_map.cpp -o build/map.o ${flags} ${include}

model: src/iso_model.cpp
	g++ -c src/iso_model.cpp -o build/model.o ${flags} ${include}

shader: src/iso_shader.cpp
	g++ -c src/iso_shader.cpp -o build/shader.o ${flags} ${include}




clean:
	rm -f build/* game