#pragma once
#include "MediaManager.h"
#include "SDL.h"

using namespace std;

class Tile {
protected:
  bool ready;
  SDL_Texture *t;

public:
  SDL_Rect rect;
  Tile(MediaManager *mm, string fname = "background.bmp") {
    ready = false;
    t = mm->get(fname);
    rect.x = 0; // the x coordinate
    rect.y = 0; // the y coordinate
    SDL_QueryTexture(t, NULL, NULL, &rect.w, &rect.h);
    ready = true;
  }
  virtual void render(SDL_Renderer *ren) {
    SDL_RenderCopy(ren, t, NULL, &rect);
  }
};
