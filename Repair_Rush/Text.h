#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

using namespace std;

class Text{
    private:
    TTF_Font* font;
    SDL_Texture* Message;
    SDL_Surface* surfaceMessage;
    SDL_Rect Message_rect;
    SDL_Renderer *renderer;
    protected:
        bool ready;
    public:
        Text(SDL_Renderer *ren, string word="Press R to start", int fontsize = 40,
            int rectx = 500, int recty = 50,int mrectx = 300,
            int mrecty = 200, string fname="./Fonts/font.ttf"){
        ready=false;
        renderer = ren;
        font = TTF_OpenFont(fname.c_str(), fontsize);
        Message_rect.x = rectx;  //controls the rect's x coordinate 
        Message_rect.y = recty; // controls the rect's y coordinte
        Message_rect.w = mrectx; // controls the width of the rect
        Message_rect.h = mrecty; // controls the height of the rect
        ready=true;
        SDL_Color White = {0, 0, 0};
        surfaceMessage =
        //TTF_RenderText_Solid(font, word.c_str(), White);
        TTF_RenderText_Blended_Wrapped(font, word.c_str(), White, 150);
        Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        }
        void display() {SDL_RenderCopy(renderer, Message, NULL, &Message_rect);}
        void destroy() {
            SDL_FreeSurface(surfaceMessage);
            SDL_DestroyTexture(Message);
        }
};

