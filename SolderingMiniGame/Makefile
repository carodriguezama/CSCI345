# Project Name
PROJ = SolderingGame
EXE = $(PROJ).exe

# Source and Header Files
SRC = $(PROJ).cpp

# Compiler and Flags
CXX = g++
CXXFLAGS = -g -Wall
SDLCFLAGS = -IC:/sdl2/include/SDL2 -D_REENTRANT
SDLLIBS = -LC:/sdl2/lib -lSDL2main -lSDL2 -mconsole -fno-use-cxa-atexit

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
