#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "SDL.h"
#include "Game.h"
#include "Tile.h"
#include "Sprite.h"
#include "Animation.h"
 
using namespace std;

class MyGame:public Game {
    float dt;
    Sprite *bf,*bf2,*bf3,*bf4;
    string tool;
    int n;
    Tile *back;
  public:
    MyGame(int level=1):Game(){
        ifstream in;
        in.open("loadtools.txt"); 
        in>>tool;
        srand(level);
        bf=new Sprite(getMM(),600,225,0,0,0,0,tool);
        in>>tool;
        bf2=new Sprite(getMM(),300,270,0,0,0,0,tool);
        in>>tool;
        bf3=new Sprite(getMM(),450,225,0,0,0,0,tool);
        in>>tool;
        bf4=new Sprite(getMM(),150,270,0,0,0,0,tool);
        cout<<tool<<endl;
        in.close();
        back=new Tile(getMM());
        dt=.01; 
    }
    void loop() {
        bf->loop(dt);
        bf2->loop(dt);
        bf3->loop(dt);
        bf4->loop(dt);
        back->render(getRen());
        bf->render(getRen());
        bf2->render(getRen());
        bf3->render(getRen());
        bf4->render(getRen());
        SDL_RenderPresent(getRen()); 
        SDL_Delay(1000.0*dt);
        SDL_Event event;
        if (SDL_PollEvent(&event)){
          if (event.type==SDL_KEYDOWN){
            if (event.key.keysym.sym==SDLK_ESCAPE) running=false;
            if (event.key.keysym.sym==SDLK_SPACE) bf->bounce();
          }
          if (event.type==SDL_WINDOWEVENT){
            if (event.window.event==SDL_WINDOWEVENT_CLOSE) running=false;
          }
        }
    }
    ~MyGame(){
      delete bf;
      delete bf2;
      delete bf3;
      delete bf4;
      delete back;
    }
};

int main(void)
{
  MyGame game(1);
  game.run();
  return 0;
}