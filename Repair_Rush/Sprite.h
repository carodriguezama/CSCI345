#pragma once
#include "SDL.h"
#include "MediaManager.h"
#include "Tile.h"

using namespace std;

class Sprite:public Tile {
   float ax,ay,vx,vy,x,y; 
   public:
   bool didCollide(Sprite &other){
    if (this==&other) return false; // can't collide with ourselves
    return ((other.x<x && x < other.x+other.rect.w) && 
            (other.y<y && y < other.y+other.rect.h));   
  }
  bool isActive(Sprite &cursor) {//funciton to check if the user has selected the tool
    if (this==&cursor) return false; // cursor cant select curspr
    return ((cursor.x<x && x < cursor.x+cursor.rect.w) && 
            (cursor.y<y && y < cursor.y+cursor.rect.h));   
  }
  Sprite(MediaManager *mm,
    float x0=0,float y0=0,
    float vx0=0,float vy0=0,
    float ax0=0,float ay0=0,string fname="face.bmp")
      :Tile(mm,fname)
      {
       // cout << "Constructor Sprite" << x0 << ' ' << y0 << endl;
       ax=ax0;
       ay=ay0;
       if (vx0==-1) vx=rand() % (100 - -100) + -100;  // rand()%(max-min)+min;
       else vx=vx0;
       if (vy0==-1) vy=rand() % (100 - -100) + -100;
       else vy=vy0;
       if (x0==-1) x=rand() % 800;
       else x=x0;
       if (y0==-1) y=rand()%600;
       else y=y0; 
    }   
   void bounceX(){ vx*=-1; }
   void bounceY() { vy*=-1; }
   void bounce(){ // Bounce in both x and y
      bounceX();
      bounceY(); 
    } 
   virtual void loop(float dt){ // dt in seconds
      vx+=ax*dt;
      vy+=ay*dt;
      x+=vx*dt;
      y+=vy*dt;
      rect.x=x;
      rect.y=y;
      if (rect.x>800-rect.w) vx*=-1;
      if (rect.x<1)vx*=-1;
      if (rect.y>600-rect.h) {
        ay*=-1;
        vy*=-1;
      }
      if (rect.y<1) {
        ay*=-1;
        vy*=-1;
      }
   }
};
