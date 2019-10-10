# Input/Output name variables
executable 	= game

# Flag variables
cpp_flags	= -std=c++11 -Wall -Werror
gl_flags	= -lGL
sfml_flags 	= -lsfml-system -lsfml-window -lsfml-graphics
misc_flags  = -ldl
flags 		= ${gl_flags} ${sfml_flags} ${misc_flags} ${cpp_flags}

# Modules
camera 		= build/camera.o
character 	= build/character.o
database	= build/database.o
input		= build/input.o
map 		= build/map.o
physics 	= build/physics.o
shader 		= build/shader.o

glad		= build/glad.o

iso_modules = ${camera} ${character} ${database} ${input} ${map} ${physics} ${shader}

# Paths
include		= -I./include
src			= ./src

# Building
all: game

game: src/main.cpp ${glad} ${iso_modules} shaders/*
	g++ src/main.cpp build/*.o -o ${executable} ${flags} ${include}

${glad}: src/glad.c
	g++ -c src/glad.c -o ${glad} ${flags} ${include}

# Project code

${camera}: src/iso_camera.cpp
	g++ -c src/iso_camera.cpp -o ${camera} ${flags} ${include}

${character}: src/iso_character.cpp
	g++ -c src/iso_character.cpp -o ${character} ${flags} ${include}

${database}: src/iso_database.cpp
	g++ -c src/iso_database.cpp -o ${database} ${flags} ${include}

${input}: src/iso_input.cpp
	g++ -c src/iso_input.cpp -o ${input} ${flags} ${include}

${map}: src/iso_map.cpp
	g++ -c src/iso_map.cpp -o ${map} ${flags} ${include}

${physics}: src/iso_physics.cpp
	g++ -c src/iso_physics.cpp -o ${physics} ${flags} ${include}

${shader}: src/iso_shader.cpp
	g++ -c src/iso_shader.cpp -o ${shader} ${flags} ${include}




clean:
	rm -f build/* game