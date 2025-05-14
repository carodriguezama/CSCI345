UNAME_S := $(shell uname -s)

ifeq ($(OS),)
  ifeq ($(UNAME_S),Linux)
    OS := Linux
  endif
  ifeq ($(UNAME_S),Darwin)
    OS := MacOS
  endif
  ifeq ($(OS),)
    OS := Windows_NT
  endif
endif

# SDL Flags by OS
ifeq ($(OS),Windows_NT)
    SDLCFLAGS = -IC:/msys64/mingw64/include/SDL2 -Dmain=SDL_main
    SDLLIBS = -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image
endif

ifeq ($(OS),MacOS)
    SDLCFLAGS=-I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -D_REENTRANT
	SDLLIBS=$(shell sdl2-config --libs) -lSDL2_mixer -lSDL2_ttf -lSDL2_image
endif

ifeq ($(OS),Linux)
    SDLCFLAGS = -I/usr/include/SDL2 -D_REENTRANT
    SDLLIBS = -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image
endif

PROJ=simple
EXE=${PROJ}
SRC=${PROJ}.cpp ControlScreen.cpp ServiceTable.cpp MotherboardMinigame.cpp SolderingGame.cpp

HEADERSRCS=Tile Sprite MediaManager Game
HEADERS=$(foreach item,$(HEADERSRCS),$(item).h)

CFLAGS= -std=c++11 -g
#SDLCFLAGS=-I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -D_REENTRANT
#SDLLIBS=$(shell sdl2-config --libs) -lSDL2_mixer -lSDL2_ttf -lSDL2_image

run: ${EXE}
	./${EXE}

${EXE}: ${SRC} ${HEADERS}
	g++ ${SDLCFLAGS} ${CFLAGS} ${SRC} -o $@ ${SDLLIBS}

clean:
	rm -f ${EXE}
