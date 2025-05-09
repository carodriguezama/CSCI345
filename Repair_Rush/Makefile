PROJ=simple
EXE=${PROJ}
SRC=${PROJ}.cpp ControlScreen.cpp ServiceTable.cpp

HEADERSRCS=Tile Sprite MediaManager Game
HEADERS=$(foreach item,$(HEADERSRCS),$(item).h)

CFLAGS=-g
SDLCFLAGS=$(shell sdl2-config --cflags)
SDLLIBS=$(shell sdl2-config --libs) -lSDL2_mixer -lSDL2_ttf -lSDL2_image

run: ${EXE}
	./${EXE}

${EXE}: ${SRC} ${HEADERS}
	g++ ${SDLCFLAGS} ${CFLAGS} ${SRC} -o $@ ${SDLLIBS}

clean:
	rm -f ${EXE}
