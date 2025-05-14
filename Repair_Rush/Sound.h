#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

using namespace std;

class Sound{
    private:
    Mix_Chunk *chunk;
    int channel,volume;
    protected:
        bool ready;
    public:
        Sound(string fname="sounds/crash.wav", int chan=-1, int vol = 3){
        ready=false;
        chunk = Mix_LoadWAV(fname.c_str());
        volume = vol;
        channel = chan;
        ready=true;
        }
        void play() {
            Mix_PlayChannel(channel, chunk, 0);
            Mix_Volume(channel, MIX_MAX_VOLUME/volume);
        }
        void loop() {Mix_PlayChannel(channel, chunk, -1);}
        void stop() {Mix_HaltChannel(channel);}
        void free() {Mix_FreeChunk(chunk);}
};

