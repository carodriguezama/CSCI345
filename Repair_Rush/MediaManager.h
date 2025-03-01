#pragma once
#include "SDL.h"
#include <map>

using namespace std;

class MediaManager {
   SDL_Renderer *ren;
   map<string,SDL_Texture *> images;
   public:
   MediaManager(SDL_Renderer *newRen) {
    ren=newRen;
  }
  SDL_Texture *get(string fname) {
    if (images.find(fname)==images.end()){
      SDL_Surface *surface = SDL_LoadBMP(fname.c_str());
      if (!surface){
         cerr << "Failed to load image"<<endl;
         return NULL;
      }
      SDL_SetColorKey(surface,SDL_TRUE,
        SDL_MapRGBA(surface->format,0,255,0,255)
      );
      SDL_Texture *t=SDL_CreateTextureFromSurface(ren,surface);
      if (!t){
        cerr << "Failed to create texture" <<endl;
        return NULL;
      }
      SDL_FreeSurface(surface);
      images[fname]=t;
      return t;
    } else {
      return images[fname];
    }
  }
};
