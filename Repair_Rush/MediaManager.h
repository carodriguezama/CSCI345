#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include <map>
using namespace std;

class MediaManager {
  SDL_Renderer *ren;
  map<string, SDL_Texture *> images;

public:
  MediaManager(SDL_Renderer *newRen) { ren = newRen; }
  SDL_Texture *get(string fname) {
    if (images.find(fname) == images.end()) {
      SDL_Surface *surface = nullptr;

      // Check file extension
      if (fname.size() >= 4 && fname.substr(fname.size() - 4) == ".bmp") {
        surface = SDL_LoadBMP(fname.c_str());
        if (!surface) {
          cerr << "Failed to load BMP: " << fname << " — " << SDL_GetError()
               << endl;
          return nullptr;
        }
      } else {
        surface = IMG_Load(fname.c_str());
        if (!surface) {
          cerr << "Failed to load image: " << fname << " — " << IMG_GetError()
               << endl;
          return nullptr;
        }
      }

      SDL_SetColorKey(surface, SDL_TRUE,
                      SDL_MapRGB(surface->format, 0, 255, 0));

      SDL_Texture *t = SDL_CreateTextureFromSurface(ren, surface);
      if (!t) {
        cerr << "Failed to create texture for: " << fname << " — "
             << SDL_GetError() << endl;
        SDL_FreeSurface(surface);
        return nullptr;
      }

      SDL_FreeSurface(surface);
      images[fname] = t;
      return t;
    } else {
      return images[fname];
    }
  }
};
