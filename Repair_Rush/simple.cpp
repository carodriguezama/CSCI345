#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "SDL.h"
#include "Game.h"
#include "Tile.h"
#include "Sprite.h"
// #include "Animation.h"
 
using namespace std;

/*
Reformatted the Sprite constructor, implemented the cursor, cursor control,
and the ability to select tools with the cursor. Removed the inclusion of
Animation.h in the makefile. Need to still make the inventory system
*/

class MyGame:public Game {
    float dt;
    int check, xpos, ypos;
    Sprite *cursor;
    vector<Sprite *> tools;
    string tool;
    int n;
    Tile *back;
  public:
    MyGame(int level=1):Game(){
        cursor = new Sprite(getMM(),"hand2.bmp",0,0);
        ifstream in("loadtools.txt");
        while(!in.eof()) {
          in>>tool>>xpos>>ypos;
          tools.push_back(new Sprite(getMM(),tool,xpos,ypos));
        }
        back=new Tile(getMM());
        dt=.01; 
    }
    void loop() {
        for(auto face:tools) face->loop(dt);
        cursor->loop(dt);
        back->render(getRen());
        cursor->render(getRen());
        for(auto face:tools)if(face->isActive())
        face->render(getRen());
 
        SDL_RenderPresent(getRen()); 
        SDL_Delay(1000.0*dt);
        SDL_Event event;
        if (SDL_PollEvent(&event)){
          if (event.type==SDL_KEYDOWN){
            if (event.key.keysym.sym==SDLK_ESCAPE) running=false;
            if (event.key.keysym.sym==SDLK_w) cursor->sety(-10);
            if (event.key.keysym.sym==SDLK_s) cursor->sety(10);
            if (event.key.keysym.sym==SDLK_a) cursor->setx(-10);
            if (event.key.keysym.sym==SDLK_d) cursor->setx(10);
            if (event.key.keysym.sym==SDLK_SPACE) {
              for(auto face:tools) {
                if(face->isActive())
              {
                if(cursor->isTouching(*face))
                {
                  if(face->isActive())
                  face->setActive(false);
                }
              }
              }
            }
          }
          if (event.type==SDL_WINDOWEVENT){
            if (event.window.event==SDL_WINDOWEVENT_CLOSE) running=false;
          }
        }

        check = 0;

        for(auto face:tools) {
          if(!face->isActive()) check++;
        }
        if(check==5) running = false;
    }
    ~MyGame(){
      for(auto face:tools) delete face;
      delete back;
      delete cursor;
    }
};

int main(void)
{
  MyGame game(1);
  game.run();
  return 0;
}