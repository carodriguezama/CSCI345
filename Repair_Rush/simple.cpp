#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Game.h"
#include "Tile.h"
#include "Sprite.h"
// #include "Animation.h"
 
using namespace std;

class MyGame:public Game {
    float dt;
    int check, xpos, ypos;
    Sprite *character, *computer;
    vector<Sprite *> tools;
    Mix_Chunk *wave,*grab;
    TTF_Font* Sans;
    SDL_Texture* Message;
    SDL_Surface* surfaceMessage;
    SDL_Rect Message_rect;
    string tool;
    int which;
    Tile *back, *repair, *cabinet, *intro;
  public:
    MyGame(int level=1):Game(){
        which = 0;
        Sans = TTF_OpenFont("./Fonts/font.ttf", 50);
        Message_rect.x = 500;  //controls the rect's x coordinate 
        Message_rect.y = 200; // controls the rect's y coordinte
        Message_rect.w = 300; // controls the width of the rect
        Message_rect.h = 300; // controls the height of the rect
        character = new Sprite(getMM(),"./Images/Character/Character.bmp",250,100);//Have three characters maybe be able to switch characters?
        ifstream in("./game_textFiles/loadtools.txt");
        while(!in.eof()) {
          in>>tool>>xpos>>ypos;
          tools.push_back(new Sprite(getMM(),tool,xpos,ypos));
        }
        back = new Tile(getMM(),"./Images/Background/background2.bmp");//Maybe a background tile vector?
        repair = new Tile(getMM(),"./Images/Background/Repair_Shop.bmp");
        cabinet = new Tile(getMM(),"./Images/Background/Desktop.bmp");
        intro = new Tile(getMM(),"./Images/Background/Cover.bmp");

        dt=.01; 
        SDL_Color White = {0, 0, 0};
        surfaceMessage =
        TTF_RenderText_Solid(Sans, "Press R to start", White); 
        wave = Mix_LoadWAV("./Sounds/footstep.wav");
        grab = Mix_LoadWAV("./Sounds/grab.wav");
        Message = SDL_CreateTextureFromSurface(getRen(), surfaceMessage);

    }
    void loop() {
        for(auto tool:tools) tool->loop(dt);
        character->loop(dt);
        if(which==0) {
          intro->render(getRen());
          SDL_RenderCopy(getRen(), Message, NULL, &Message_rect);
        }
        else if(which==1) {
        character->setActive(true);
        repair->render(getRen());
        character->render(getRen());
        tools[6]->setActive(false);
        }
        else if(which==2) {
        cabinet->render(getRen());
        tools[5]->render(getRen());
        }
        else {
        tools[6]->setActive(true);
        back->render(getRen());
        tools[6]->render(getRen());
        character->setActive(false);
        for(int i = 0;i<5;i++)if(tools[i]->isActive())tools[i]->render(getRen());
        }
 
        SDL_RenderPresent(getRen()); 
        SDL_Delay(1000.0*dt);
        SDL_Event event;
        if (SDL_PollEvent(&event)){
          if (event.type==SDL_KEYDOWN){
            if (event.key.keysym.sym==SDLK_ESCAPE) running=false;
            if (event.key.keysym.sym==SDLK_w) {if(tools[6]->isActive()) tools[6]->sety(-10); 
              else {
                character->sety(-10);
                Mix_PlayChannel(-1, wave, 0);
              }
            }
            if (event.key.keysym.sym==SDLK_s) {if(tools[6]->isActive()) tools[6]->sety(10); 
              else {
                character->sety(10);
                Mix_PlayChannel(-1, wave, 0);
              }
            }
            if (event.key.keysym.sym==SDLK_a) {if(tools[6]->isActive())tools[6]->setx(-10); 
              else {
                character->setx(-10);
                Mix_PlayChannel(-1, wave, 0);
              }
            }
            if (event.key.keysym.sym==SDLK_d) {if(tools[6]->isActive())tools[6]->setx(10); 
              else {
                character->setx(10);
                Mix_PlayChannel(-1, wave, 0);
              }
            }
            if (event.key.keysym.sym==SDLK_r) which = 1;//repair
            if (event.key.keysym.sym==SDLK_b) which = 2;//bench
            if (event.key.keysym.sym==SDLK_c) which = 3;//Cabinet
            if (event.key.keysym.sym==SDLK_SPACE) {
              for(int i = 0;i<5;i++){
                if(tools[i]->isActive())
              {
                if(tools[6]->isTouching(*tools[i]))
                {
                  if(tools[i]->isActive())
                  tools[i]->setActive(false);
                  Mix_PlayChannel(-1, grab, 0);

                }
              }
              }
            }
          }
          if (event.type==SDL_WINDOWEVENT){
            if (event.window.event==SDL_WINDOWEVENT_CLOSE) running=false;
          }
        }

    }
    ~MyGame(){
      Mix_FreeChunk(wave);
      Mix_FreeChunk(grab);
      for(auto tool:tools) delete tool;
      delete back;
      delete repair;
      delete cabinet;
      delete character;
      SDL_FreeSurface(surfaceMessage);
      SDL_DestroyTexture(Message);
    }
};

int main(void)
{
  MyGame game(1);
  game.run();
  return 0;
}