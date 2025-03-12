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
    Sprite /**cursor,*/ *character, *computer;
    vector<Sprite *> tools;
    string tool;
    int which;
    Tile *back, *repair, *cabinet, *intro;
  public:
    MyGame(int level=1):Game(){
        which = 0;
        //cursor = new Sprite(getMM(),"hand2.bmp",0,0);//Could maybe add this to the tools vector
        //computer = new Sprite(getMM(),"Computer.bmp",250,10);//Could add this to the tools vector
        character = new Sprite(getMM(),"./Images/Character/Character.bmp",250,100);//Have three characters maybe be able to switch characters?
        ifstream in("loadtools.txt");
        while(!in.eof()) {
          in>>tool>>xpos>>ypos;
          tools.push_back(new Sprite(getMM(),tool,xpos,ypos));
        }
        back = new Tile(getMM(),"background2.bmp");//Maybe a background tile vector?
        repair = new Tile(getMM(),"Repair_Shop.bmp");
        cabinet = new Tile(getMM(),"Desktop.bmp");
        intro = new Tile(getMM(),"Cover.bmp");

        dt=.01; 
    }
    void loop() {
        for(auto face:tools) face->loop(dt);
        //cursor->loop(dt);
        character->loop(dt);
        //computer->loop(dt);
        if(which==0) {
          intro->render(getRen());
        }
        else if(which==1) {
        character->setActive(true);
        repair->render(getRen());
        character->render(getRen());
        //cursor->setActive(false);
        tools[6]->setActive(false);
        }
        else if(which==2) {
        cabinet->render(getRen());
        //computer->render(getRen());
        tools[5]->render(getRen());
        }
        else {
        //cursor->setActive(true);
        tools[6]->setActive(true);
        back->render(getRen());
        tools[6]->render(getRen());
        //cursor->render(getRen());
        character->setActive(false);
        for(int i = 0;i<5;i++)if(tools[i]->isActive())tools[i]->render(getRen());
        /*for(auto face:tools)if(face->isActive())
        face->render(getRen());*/
        }
 
        SDL_RenderPresent(getRen()); 
        SDL_Delay(1000.0*dt);
        SDL_Event event;
        if (SDL_PollEvent(&event)){
          if (event.type==SDL_KEYDOWN){
            if (event.key.keysym.sym==SDLK_ESCAPE) running=false;
            if (event.key.keysym.sym==SDLK_w) {if(tools[6]->isActive()) tools[6]->sety(-10); 
              else character->sety(-10);}
            if (event.key.keysym.sym==SDLK_s) {if(tools[6]->isActive()) tools[6]->sety(10); 
              else character->sety(10);}
            if (event.key.keysym.sym==SDLK_a) {if(tools[6]->isActive())tools[6]->setx(-10); 
              else character->setx(-10);}
            if (event.key.keysym.sym==SDLK_d) {if(tools[6]->isActive())tools[6]->setx(10); 
              else character->setx(10);}
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
                }
              }
              }
            }
          }
          if (event.type==SDL_WINDOWEVENT){
            if (event.window.event==SDL_WINDOWEVENT_CLOSE) running=false;
          }
        }

        /*check = 0;

        for(auto face:tools) {
          if(!face->isActive()) check++;
        }
        if(check==5) running = false;*/
    }
    ~MyGame(){
      for(auto face:tools) delete face;
      delete back;
      delete repair;
      delete cabinet;
      delete character;
     // delete cursor;
      //delete computer;
    }
};

int main(void)
{
  MyGame game(1);
  game.run();
  return 0;
}