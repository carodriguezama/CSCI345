// #pragma once
// #include <vector>
// #include <istream>
// #include "SDL.h"
// #include "MediaManager.h"
// #include "Tile.h"

// using namespace std;

// class AnimationFrame {
//     public:
//     Tile *t;
//     int time;
//     AnimationFrame(MediaManager *mm,string fname,int frameTime){
//         t=new Tile(mm,fname);
//         time=frameTime;
//     }
// };

// class Animation:public Sprite {
//   vector<AnimationFrame *> tiles;
//   int current,animationTotal;
//   float total;
//   public:
//   /*Animation(const Animation *other){
//     // make a copy of everything in the other animation
//   }*/
//   Animation(MediaManager *mm,istream &in,float x0=0,float y0=0,
//     float vx0=0,float vy0=0,
//     float ax0=0,float ay0=0):Sprite(mm,x0,y0,vx0,vy0,ax0,ay0){
//     current=0;
//     animationTotal=0;
//     total=0.0;
//     int n;
//     in >> n;
//     for (int i=0;i<n;i++){
//         string fname;
//         int frameTime;
//         in >> fname >> frameTime;
//         animationTotal+=frameTime;
//         tiles.push_back(new AnimationFrame(mm,fname,frameTime));
//     }
//     cout << "Animation load finished"<<endl;
//   }
//   void loop(float dt){ // dt is in seconds
//     Sprite::loop(dt);
//     total+=dt;
//     int totalms=total*1000; // totalms for all time
//     totalms=totalms % animationTotal;  // totalms within this animation
//     for (int i=0;i<tiles.size();i++){
//         if (totalms<tiles[i]->time){
//             current=i;
//             break;
//         }
//         totalms-=tiles[i]->time;
//     }
//     tiles[current]->t->rect.x = rect.x;
//     tiles[current]->t->rect.y = rect.y;
//   }
//   void render(SDL_Renderer *ren){ 
//     tiles[current]->t->render(ren); 
//   }
// };