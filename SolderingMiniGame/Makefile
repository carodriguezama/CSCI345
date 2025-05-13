# Project Name
PROJ = SolderingGame
EXE = $(PROJ).exe

# Source and Header Files
SRC = $(PROJ).cpp

# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
SDLCFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -D_REENTRANT
SDLLIBS =$(shell sdl2-config --libs) -lSDL2main -lSDL2 -mconsole -fno-use-cxa-atexit

# Build Rules
all: $(EXE)

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) $(SDLCFLAGS) $(SRC) -o $@ $(SDLLIBS)

# Run the program
run: $(EXE)
	$(EXE)

# Clean up build files
clean:
	del /f $(EXE)
