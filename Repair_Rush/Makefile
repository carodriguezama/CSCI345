
PROJ=simple
EXE=${PROJ}
SRC=${PROJ}.cpp ControlScreen.cpp ServiceTable.cpp MotherboardMinigame.cpp SolderingGame.cpp

HEADERSRCS=Tile Sprite MediaManager Game
HEADERS=$(foreach item,$(HEADERSRCS),$(item).h)

CFLAGS= -std=c++11 -g
SDLCFLAGS=-I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -D_REENTRANT
SDLLIBS=$(shell sdl2-config --libs) -lSDL2_mixer -lSDL2_ttf -lSDL2_image

run: ${EXE}
	./${EXE}

${EXE}: ${SRC} ${HEADERS}
	g++ ${SDLCFLAGS} ${CFLAGS} ${SRC} -o $@ ${SDLLIBS}

clean:
	rm -f ${EXE}
