#Makefile for the nerezza rpg game

#28/10/2006 Stephan Luther

CXX=gcc

COMPILE=-c -ggdb -Wall -fno-inline

OBJECTS=splash.o voxel.o plasma.o

SDL_FLAGS=$(shell sdl-config --cflags) 
#SDL_LIBS=$(shell sdl-config --libs) -lSDL_net
SDL_LIBS=$(shell sdl-config --libs)

help:
	@echo "\"make all\" Will make everything"
	@echo "\"make voxel\" Will make the voxel demo"
	@echo "\"make bitmap\" Will make the bitmap test"
	@echo "\"make clean\" Will delete all generated files"

all:voxel bitmap
	@echo "Built all"

voxel:$(OBJECTS) main.o
	@echo "Building voxel demo"
	#$(CXX) -o voxel $(OBJECTS) main.o $(SDL_LIBS) -gdbb -lglut -lm
	$(CXX) -o voxel $(OBJECTS) main.o $(SDL_LIBS) -gdbb -lm

bitmap: splash.o bitmap_test.o
	@echo "Building the bitmap test harness"
	#$(CXX) -o bitmap splash.o bitmap_test.o $(SDL_LIBS) -gdbb -lglut -lm
	$(CXX) -o bitmap splash.o bitmap_test.o $(SDL_LIBS) -gdbb -lm

main.o: main.c
	$(CXX) $(COMPILE) main.c $(SDL_FLAGS)

splash.o: splash.c splash.h
	$(CXX) $(COMPILE) splash.c $(SDL_FLAGS)

voxel.o: voxel.c voxel.h
	$(CXX) $(COMPILE) voxel.c $(SDL_FLAGS)

plasma.o: plasma.c plasma.h
	$(CXX) $(COMPILE) plasma.c $(SDL_FLAGS)

bitmap_test.o: bitmap_test.c
	$(CXX) $(COMPILE) bitmap_test.c $(SDL_FLAGS)

clean:
	rm -f *.o voxel bitmap *.core core
