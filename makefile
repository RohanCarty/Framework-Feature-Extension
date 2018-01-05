CC=g++

CFLAGS=-c -g -Wall `xml2-config --cflags` `sdl2-config --cflags` -I include/

LDFLAGS=-lGLEW -lGL `sdl2-config --libs` `xml2-config --libs` -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lSDL2_image

SOURCES=$(wildcard src/*.cpp)

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=bin/PegasusFeather.x86_64



all: $(SOURCES) $(EXECUTABLE)

	

$(EXECUTABLE): $(OBJECTS) 

	$(CC) $(OBJECTS) $(LDFLAGS) -o $@



.cpp.o:

	$(CC) $(CFLAGS) $< -o $@

	

clean: 

	rm -rf $(OBJECTS)

