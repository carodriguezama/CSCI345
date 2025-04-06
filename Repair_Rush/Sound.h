#pragma once
#include "SDL.h"
#include "SDL_mixer.h"

using namespace std;

class Sound{
    private:
    Mix_Chunk *chunk;
    protected:
        bool ready;
    public:
        Sound(string fname="./Sounds/footstep.wav"){
        ready=false;
        chunk = Mix_LoadWAV(fname.c_str());
        ready=true;
        }
        void play() {Mix_PlayChannel(-1, chunk, 0);}
        void free() {Mix_FreeChunk(chunk);}
};

