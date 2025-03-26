ifeq ($(OS),Windows_NT) 
#	SDLCFLAGS=<your cflags Windows flags go here>
#	SDLLIBS=<your libs Windows flags go here>
#	Your flags should look something like below
	WINLOC=$(GAMELOC)
	SDLCFLAGS=`sdl2-config --cflags`
	SDLLIBS=`sdl2-config --libs` -lSDL2_mixer -lSDL2_ttf
#	SDLCFLAGS=-I$(WINLOC)/msys64/mingw64/include/SDL2/ -Dmain=SDL_main
#	SDLLIBS=-L$(WINLOC)/msys64/mingw64/lib/ -lmingw32 -mwindows -lSDL2main -lSDL2
endif

ifeq ($(OS),MacOS)
#		SDLCFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
#		SDLLIBS=-L/opt/homebrew/lib -lSDL2 -lSDL2_mixer -lSDL2_ttf
		SDLCFLAGS=`sdl2-config --cflags`
		SDLLIBS=`sdl2-config --libs` -lSDL2_mixer -lSDL2_ttf -lSDL2_image
endif

ifeq ($(OS),Linux)
	SDLCFLAGS=-I/usr/include/SDL2 -D_REENTRANT
	SDLLIBS=-lSDL2
endif

PROJ=simple
EXE=${PROJ}.exe
SRC=${PROJ}.cpp

HEADERSRCS=Tile Sprite MediaManager Game
HEADERS=$(foreach item,$(HEADERSRCS),$(item).h)
CFLAGS=-g 
#SDLCFLAGS=-I/usr/include/SDL2 -D_REENTRANT
#SDLLIBS=-lSDL2 -lSDL2_mixer -lSDL2_ttf

run: ${EXE}
	./${EXE}

${EXE}:${SRC} ${OBJS} ${HEADERS}
	g++  ${SDLCFLAGS} ${CFLAGS} $<  -o $@ ${OBJS} ${SDLLIBS} 

clean:
	rm ${EXE} ${OBJS}

