# Input/Output name variables
executable 	= game

# Flag variables
gl_flags	= -lGL
sfml_flags 	= -lsfml-system -lsfml-window
misc_flags  = -ldl
flags 		= ${gl_flags} ${sfml_flags} ${misc_flags}

# Paths
include		= ./include
src			= ./src

# Building
all: game

game: src/main.cpp src/glad.c
	g++ src/main.cpp src/glad.c -o ${executable} ${flags} -I${include}

clean:
	rm -f *.o game