#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "SDL.h"
#include "Game.h"
#include "Tile.h"
#include "Sprite.h"
#include "Sound.h"
#include "Text.h"
// #include "Animation.h"
 
using namespace std;

class MyGame:public Game {
    float dt;
    int check, xpos, ypos;
    vector<Sprite *> tools;
    vector<Sprite *> characters;
    Sound *wave,*grab;
    Text *text;
    string tool;
    bool start;
    int which;
    Tile *back, *repair, *cabinet, *intro, *select;
  public:
    MyGame(int level=1):Game(){
        which = 0;
        start = true;
        ifstream in("./game_textFiles/loadtools.txt");
        while(!in.eof()) {
          in>>tool>>xpos>>ypos;
          cout<<tool<<endl;
          tools.push_back(new Sprite(getMM(),tool,xpos,ypos));
        }
        in.close();
        ifstream cin("./game_textFiles/Character.txt");
        while(!cin.eof()) {
          cin>>tool>>xpos>>ypos;
          cout<<tool<<endl;
          characters.push_back(new Sprite(getMM(),tool,xpos,ypos));
        }
        cin.close();
        back = new Tile(getMM(),"./Images/Background/background2.bmp");//Maybe a background tile vector?
        repair = new Tile(getMM(),"./Images/Background/Repair_Shop.bmp");//clean this up
        cabinet = new Tile(getMM(),"./Images/Background/Desktop.bmp");
        intro = new Tile(getMM(),"./Images/Background/Cover.bmp");
        select = new Tile(getMM(),"./Images/Background/Select.bmp");
        wave = new Sound();
        grab = new Sound("./Sounds/grab.wav");
        text = new Text(getRen(),"PRESS ENTER");
        dt=.01; 

    }
    void loop() {
        for(auto tool:tools) tool->loop(dt);//loading screen -> choose character -> Client
        for(auto tool:characters) tool->loop(dt);//loading screen -> choose character -> Client

        if (which==0) {//make a class for settings and controls
          intro->render(getRen());
          text->display();
        }
        else if(which==1) {
          select->render(getRen());
          for(int i = 0;i<3;i++)/*if(characters[i]->isActive())*/characters[i]->render(getRen());
        }
        else if(which==2) {
        repair->render(getRen());
        for(int i = 0;i<3;i++)if(characters[i]->isActive()) characters[i]->render(getRen());
        tools[6]->setActive(false);
        }
        else if(which==3) {
        cabinet->render(getRen());
        tools[5]->render(getRen());
        }
        else {
        tools[6]->setActive(true);
        back->render(getRen());
        tools[6]->render(getRen());
        for(int i = 0;i<5;i++)if(tools[i]->isActive())tools[i]->render(getRen());
        }
 
        SDL_RenderPresent(getRen()); 
        SDL_Delay(1000.0*dt);
        SDL_Event event;
        if (SDL_PollEvent(&event)){
          if (event.type==SDL_KEYDOWN){
            if (event.key.keysym.sym==SDLK_1) {
              characters[1]->setActive(false);//repair
              characters[2]->setActive(false);
              which = 2;
            }
            if (event.key.keysym.sym==SDLK_2) {
              characters[0]->setActive(false);//repair
              characters[2]->setActive(false);
              which = 2;
            }
            if (event.key.keysym.sym==SDLK_3) {
              characters[0]->setActive(false);//repair
              characters[1]->setActive(false);
              which = 2;
            }
            if (event.key.keysym.sym==SDLK_RETURN) which = 1;//repair
            if (event.key.keysym.sym==SDLK_r) which = 2;//repair
            if (event.key.keysym.sym==SDLK_b) which = 3;//bench
            if (event.key.keysym.sym==SDLK_c) which = 4;//Cabinet
            if (event.key.keysym.sym==SDLK_ESCAPE) running=false;
            if (event.key.keysym.sym==SDLK_w) {if(tools[6]->isActive()) tools[6]->sety(-10); 
              else {
               for(int i = 0;i<3;i++)if(characters[i]->isActive()) characters[i]->sety(-10);
               wave->play();
              }
            }
            if (event.key.keysym.sym==SDLK_s) {if(tools[6]->isActive()) tools[6]->sety(10); 
              else {
                for(int i = 0;i<3;i++)if(characters[i]->isActive()) characters[i]->sety(10);
                wave->play();
              }
            }
            if (event.key.keysym.sym==SDLK_a) {if(tools[6]->isActive())tools[6]->setx(-10); 
              else {
                for(int i = 0;i<3;i++)if(characters[i]->isActive()) characters[i]->setx(-10);
                wave->play();
              }
            }
            if (event.key.keysym.sym==SDLK_d) {if(tools[6]->isActive())tools[6]->setx(10); 
              else {
                for(int i = 0;i<3;i++)if(characters[i]->isActive()) characters[i]->setx(10);
                wave->play();
              }
            }
            if (event.key.keysym.sym==SDLK_SPACE) {
              for(int i = 0;i<5;i++){
              if(tools[i]->isActive()) {
                  tools[i]->setActive(!tools[6]->isTouching(*tools[i]));
                  grab->play();
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
      wave->free();
      grab->free();
      for(auto tool:tools) delete tool;
      for(auto tool:characters) delete tool;
      delete back;
      delete repair;
      delete cabinet;
      delete select;
      text->destroy();
    }
};

int main(void)
{
  MyGame game(1);
  game.run();
  return 0;
}