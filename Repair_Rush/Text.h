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
        Text(SDL_Renderer *ren, string word="Press R to start", int fontsize = 15,
            int rectx = 500, int recty = 50, bool white = true, string fname="./Fonts/BungeeSpice-Regular.ttf"){
        ready=false;
        renderer = ren;
        font = TTF_OpenFont(fname.c_str(), fontsize);
        SDL_Color Color = {0, 0, 0};
        if(white) SDL_Color Color = {255, 255, 255};
        surfaceMessage =
        TTF_RenderText_Solid(font, word.c_str(), Color);
        Message_rect.x = rectx;  //controls the rect's x coordinate 
        Message_rect.y = recty; // controls the rect's y coordinte
        Message_rect.w = surfaceMessage->w; // controls the width of the rect
        Message_rect.h = surfaceMessage->h; // controls the height of the rectsurfaceMessage
        
        ready=true;
        Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        }
        void display() {SDL_RenderCopy(renderer, Message, NULL, &Message_rect);}
        void destroy() {
            SDL_FreeSurface(surfaceMessage);
            SDL_DestroyTexture(Message);
        }
};

