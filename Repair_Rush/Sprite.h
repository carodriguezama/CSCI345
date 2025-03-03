#pragma once
#include "SDL.h"
#include "MediaManager.h"
#include "Tile.h"

using namespace std;

/*
Removing acceleration and random velocity generation. Making default velocity 0
Renaming isActive to be isTouching to see if the cursor is over a tool
Made new function isActive to return value of new member bool active
Made functions for setting active, vy, and vx
*/
class Sprite:public Tile {
   float vx,vy,x,y; 
   bool active;
   public:
  //  bool didCollide(Sprite &other){
  //   if (this==&other) return false; // can't collide with ourselves
  //   return ((other.x<x && x < other.x+other.rect.w) && 
  //           (other.y<y && y < other.y+other.rect.h));   
  // }
  bool isTouching(Sprite &tool) {//funciton to check if the user has selected the tool
    if (this==&tool) return false; // cursor cant select cursor
    return ((tool.x<x && x < tool.x+tool.rect.w) && 
            (tool.y<y && y < tool.y+tool.rect.h));   
  }

  bool isActive()
  {
    return active;
  }

  void setActive(bool b)
  {
    active = b;
  }

  void setVy(float vy0)
  {
    vy += vy0;
  }

  void setVx(float vx0)
  {
    vx += vx0;
  }

  Sprite(MediaManager *mm, string fname="face.bmp", float x0=0,float y0=0,
    float vx0=0,float vy0=0):Tile(mm,fname)
    {
        active = true;
       // cout << "Constructor Sprite" << x0 << ' ' << y0 << endl;
      //  if (vx0==-1) vx=rand() % (100 - -100) + -100;  // rand()%(max-min)+min;
      //  else
        vx=vx0;
      //  if (vy0==-1) vy=rand() % (100 - -100) + -100;
      //  else
        vy=vy0;
      //  if (x0==-1) x=rand() % 800;
      //  else
        x=x0;
      //  if (y0==-1) y=rand()%600;
      //  else
        y=y0; 
    }   
  //  void bounceX(){ vx*=-1; }
  //  void bounceY() { vy*=-1; }
  //  void bounce(){ // Bounce in both x and y
  //     bounceX();
  //     bounceY(); 
    //} 
   virtual void loop(float dt){ // dt in seconds
      x+=vx*dt;
      y+=vy*dt;
      rect.x=x;
      rect.y=y;
      // if (rect.x>800-rect.w) vx*=-1;
      // if (rect.x<1)vx*=-1;
      // if (rect.y>600-rect.h) {
      //   vy*=-1;
      // }
      // if (rect.y<1) {
      //   vy*=-1;
      // }
   }
};
