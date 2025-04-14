#pragma once
#include "MediaManager.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <map>

using namespace std;

void RRQuit() {
  cerr << "You Win" << endl;
  SDL_Quit();
}

class Game {
  SDL_Window *window;
  SDL_Renderer *ren;
  MediaManager *mm;

protected:
  bool running;

public:
  SDL_Renderer *getRen() { return ren; }
  MediaManager *getMM() { return mm; }
  Game() {
    running = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
      cerr << endl << "Unable to initialize SDL: " << SDL_GetError() << endl;
      running = false;
      return;
    }
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
      running = false;
      return;
    }
    if (TTF_Init() < 0) {
      printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
      exit(1);
    }
    atexit(RRQuit);
    window = SDL_CreateWindow("Repair Rush", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window) {
      cerr << "Failed to create window" << endl;
      running = false;
      return;
    }
    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
      cerr << "Failed to create Renderer" << endl;
      running = false;
      return;
    }
    mm = new MediaManager(ren);
  }
  void run() {
    while (running) {
      loop();
    }
  }
  virtual void loop() = 0;
  ~Game() {
    delete mm;
    SDL_DestroyRenderer(ren);
  }
};
