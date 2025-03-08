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

class MyGame:public Game {
    float dt;
    int check, xpos, ypos;
    Sprite *cursor, *character;
    vector<Sprite *> tools;
    string tool;
    int which;
    Tile *back, *repair, *cabinet;
  public:
    MyGame(int level=1):Game(){
        which = 1;
        cursor = new Sprite(getMM(),"hand2.bmp",0,0);
        character = new Sprite(getMM(),"Character.bmp",0,0);
        ifstream in("loadtools.txt");
        while(!in.eof()) {
          in>>tool>>xpos>>ypos;
          tools.push_back(new Sprite(getMM(),tool,xpos,ypos));
        }
        back = new Tile(getMM());
        repair = new Tile(getMM(),"Repair_Shop.bmp");
        cabinet = new Tile(getMM(),"Desktop.bmp");

        dt=.01; 
    }
    void loop() {
        for(auto face:tools) face->loop(dt);
        cursor->loop(dt);
        character->loop(dt);
        if(which==1) {
        repair->render(getRen());
        character->render(getRen());
        }
        else if(which==2) {
        cabinet->render(getRen());
        }
        else {
        back->render(getRen());
        cursor->render(getRen());
        for(auto face:tools)if(face->isActive())
        face->render(getRen());

        }
 
        SDL_RenderPresent(getRen()); 
        SDL_Delay(1000.0*dt);
        SDL_Event event;
        if (SDL_PollEvent(&event)){
          if (event.type==SDL_KEYDOWN){
            if (event.key.keysym.sym==SDLK_ESCAPE) running=false;
            if (event.key.keysym.sym==SDLK_w) {cursor->sety(-10); character->sety(-10);}
            if (event.key.keysym.sym==SDLK_s) {cursor->sety(10); character->sety(10);}
            if (event.key.keysym.sym==SDLK_a) {cursor->setx(-10); character->setx(-10);}
            if (event.key.keysym.sym==SDLK_d) {cursor->setx(10); character->setx(10);}
            if (event.key.keysym.sym==SDLK_r) which = 1;//repair
            if (event.key.keysym.sym==SDLK_b) which = 2;//bench
            if (event.key.keysym.sym==SDLK_c) which = 3;//Cabinet
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