CFLAGS = $(shell sdl-config --cflags) -Wall
CXXFLAGS = $(CFLAGS) $(X_CFLAGS)
LIBS = -lSDL_image -lSDL_ttf -lSDL_mixer $(shell sdl-config --libs)

test: p2.o
	gcc $(CFLAGS) -o p2 p2.cpp $(LIBS) -lstdc++ -std=c++11
