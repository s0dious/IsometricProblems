executable 	= game
flags 		= -lGL -lsfml-system -lsfml-window

all: game

game: main.cpp
	g++ main.cpp -o ${executable} ${flags}