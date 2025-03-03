PROJ=simple
EXE=${PROJ}.exe
SRC=${PROJ}.cpp
#SRCS=
#CPPFiles=$(foreach item,$(SRCS),$(item).cpp)
#OBJS=$(foreach item,$(SRCS),$(item).o)
HEADERSRCS=Tile Sprite MediaManager Game
HEADERS=$(foreach item,$(HEADERSRCS),$(item).h)
CFLAGS=-g 
SDLCFLAGS=-I/usr/include/SDL2 -D_REENTRANT
SDLLIBS=-lSDL2

#%.o:%.cpp
#	g++ -c ${SDLCFLAGS} ${CFLAGS} $< -o $@ 

run: ${EXE}
	./${EXE}

${EXE}:${SRC} ${OBJS} ${HEADERS}
	g++  ${SDLCFLAGS} ${CFLAGS} $<  -o $@ ${OBJS} ${SDLLIBS} 

clean:
	rm ${EXE} ${OBJS}

