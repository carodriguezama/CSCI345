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
    if (!t) {
      cerr << "Tile failed to load: " << fname << endl;
      return;
    }

    rect.x = 0;
    rect.y = 0;
    SDL_QueryTexture(t, NULL, NULL, &rect.w, &rect.h);
    ready = true;
  }

  virtual void render(SDL_Renderer *ren) {
    if (ready && t) {
      SDL_RenderCopy(ren, t, NULL, &rect);
    } else {
      std::cerr << "Render failed — texture not ready." << std::endl;
    }
  }
  void render(SDL_Renderer *ren, double angle) {
    SDL_RenderCopyEx(ren, t, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
  }
};
