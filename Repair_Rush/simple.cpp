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

/*
Reformatted the Sprite constructor, implemented the cursor, cursor control,
and the ability to select tools with the cursor. Removed the inclusion of
Animation.h in the makefile. Need to still make the inventory system
*/

class MyGame:public Game {
    float dt;
    Sprite *bf,*bf2,*bf3,*bf4,*bf5, *cursor;//create a vector for the tools later on
    string tool;
    int n;
    Tile *back;
  public:
    MyGame(int level=1):Game(){
        ifstream in;
        cursor = new Sprite(getMM(),"hand2.bmp",0,0);
        in.open("loadtools.txt"); //do this in one function like the animation
        in>>tool;
        srand(level);
        bf=new Sprite(getMM(),tool,600,225);
        in>>tool;
        bf2=new Sprite(getMM(),tool,300,270);
        in>>tool;
        bf3=new Sprite(getMM(),tool,450,225);
        in>>tool;
        bf4=new Sprite(getMM(),tool,150,270);
        in>>tool;
        bf5=new Sprite(getMM(),tool,600,25);
        in.close();
        back=new Tile(getMM());
        dt=.01; 
    }
    void loop() {
        bf->loop(dt);
        bf2->loop(dt);
        bf3->loop(dt);
        bf4->loop(dt);
        bf5->loop(dt);
        cursor->loop(dt);
        back->render(getRen());
        cursor->render(getRen());
        //if the tool is active, render on screen
        //if not, do not render
        if(bf->isActive())
          bf->render(getRen());
        if(bf2->isActive())
          bf2->render(getRen());
        if(bf3->isActive())
          bf3->render(getRen());
        if(bf4->isActive())
          bf4->render(getRen());
        if(bf5->isActive())
          bf5->render(getRen());
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
            if (event.key.keysym.sym==SDLK_SPACE)
            {
              if(bf->isActive())
              {
                if(cursor->isTouching(*bf))
                {
                  if(bf->isActive())
                  bf->setActive(false);
                  cout << "Soldering Iron Selected\n";
                }
              }

              if(bf2->isActive())
              {
                if(cursor->isTouching(*bf2))
                {
                  bf2->setActive(false);
                  cout << "Air Canister Selected\n";
                }
              }

              if(bf3->isActive())
              {
                if(cursor->isTouching(*bf3))
                {
                  bf3->setActive(false);
                  cout << "Thermal Paste Selected\n";
                }
              }

              if(bf4->isActive())
              {
                if(cursor->isTouching(*bf4))
                {
                  bf4->setActive(false);
                  cout << "Tweezers Selected\n";
                }
              }
              if(bf5->isActive())
              {
                if(cursor->isTouching(*bf5))
                {
                  bf5->setActive(false);
                  cout << "ScrewDriver Selected\n";
                }
              }
              else
                cout << "None selected\n";
            }
            // if (event.key.keysym.sym==SDLK_SPACE) bf->bounce();
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
      delete bf5;
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