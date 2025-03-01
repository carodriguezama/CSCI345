#pragma once
#include <map>
#include "SDL.h"
#include "MediaManager.h"
using namespace std;

void RRQuit() {
  cerr << "You Win" <<endl;
  SDL_Quit();
}

class Game {
  SDL_Window *window;
  SDL_Renderer *ren;
  MediaManager *mm;
  protected:
  bool running;
  public:
  SDL_Renderer *getRen(){ return ren;}
  MediaManager *getMM() {return mm; }
  Game(){
    running=true;
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
      cerr << endl << "Unable to initialize SDL: " <<SDL_GetError()<<endl;
      running=false;
      return;
    }
    atexit(RRQuit);
    window = SDL_CreateWindow("Repair Rush",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800, 600,0);
    if(!window){
        cerr << "Failed to create window"<<endl;
        running=false;
        return;
    }
    ren=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if (!ren){
        cerr << "Failed to create Renderer"<<endl;
        running=false;
        return;
    }
    mm=new MediaManager(ren);
  }
  void run(){
    while (running){
      loop();
    }
  }
  virtual void loop()=0;
  ~Game() {
    delete mm;
    SDL_DestroyRenderer(ren);
  }
};
