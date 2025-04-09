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
    vector<Tile *> background;
    Sound *wave,*grab;
    Text *text, *help;
    string tool;
    bool start;
    int which;
  public:
    MyGame(int level=1):Game(){
        which = 0;
        start = true;
        ifstream in("./game_textFiles/loadtools.txt");
        while(!in.eof()) {
          in>>tool>>xpos>>ypos;
          tools.push_back(new Sprite(getMM(),tool,xpos,ypos));
        }
        in.close();
        ifstream cin("./game_textFiles/Character.txt");
        while(!cin.eof()) {
          cin>>tool>>xpos>>ypos;
          characters.push_back(new Sprite(getMM(),tool,xpos,ypos));
        }
        cin.close();
        in.close();
        ifstream bin("./game_textFiles/loadbackground.txt");
        while(!bin.eof()) {
          bin>>tool;
          cout<<tool<<endl;
          background.push_back(new Tile(getMM(),tool));
        }
        bin.close();
        wave = new Sound("./Sounds/footstep.wav");
        grab = new Sound("./Sounds/grab.wav");
        text = new Text(getRen(),"PRESS ENTER");
        help = new Text(getRen(),"please help me my computer is overheating",25,450,50,400,300);
        dt=.01; 

    }
    void loop() {
        for(auto tool:tools) tool->loop(dt);//loading screen -> choose character -> Client
        for(auto tool:characters) tool->loop(dt);//loading screen -> choose character -> Client

        if (which==0) {//make a class for settings and controls
          background[0]->render(getRen());
          text->display();
        }
        else if(which==1) {
          background[1]->render(getRen());
          for(int i = 0;i<3;i++) characters[i]->render(getRen());
        }
        else if(which==2) {
          background[3]->render(getRen());
          tools[5]->render(getRen());
          help->display();
        }
        else if(which==3) {
        background[2]->render(getRen());
        for(int i = 0;i<3;i++)if(characters[i]->isActive()) characters[i]->render(getRen());
        tools[6]->setActive(false);
        }
        else {
        tools[6]->setActive(true);
        background[4]->render(getRen());
        for(int i = 0;i<5;i++)if(tools[i]->isActive())tools[i]->render(getRen());
        tools[6]->render(getRen());
        }
 
        SDL_RenderPresent(getRen()); 
        SDL_Delay(1000.0*dt);
        SDL_Event event;
        if (SDL_PollEvent(&event)){
          if (event.type==SDL_KEYDOWN){
            if (event.key.keysym.sym==SDLK_1) {
              characters[1]->setActive(false);
              characters[2]->setActive(false);
              which = 2;
            }
            if (event.key.keysym.sym==SDLK_2) {
              characters[0]->setActive(false);
              characters[2]->setActive(false);
              which = 2;
            }
            if (event.key.keysym.sym==SDLK_3) {
              characters[0]->setActive(false);
              characters[1]->setActive(false);
              which = 2;
            }
            if (event.key.keysym.sym==SDLK_RETURN) which = 1;//choose character
            if (event.key.keysym.sym==SDLK_r) which = 3;//repair
            if (event.key.keysym.sym==SDLK_b) which = 2;//client
            if (event.key.keysym.sym==SDLK_c) which = 4;//tool select
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
      for(auto tool:background) delete tool;
      text->destroy();
      help->destroy();
    }
};

int main(void)
{
  MyGame game(1);
  game.run();
  return 0;
}
