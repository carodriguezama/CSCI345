#include "Game.h"
#include "SDL.h"
#include "Sound.h"
#include "Sprite.h"
#include "Text.h"
#include "ControlScreen.h" // -c
#include "Tile.h"
#include "MotherboardMinigame.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <ctime>

// #include "Animation.h"

using namespace std;
Mix_Music* music = nullptr; // Background music track
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
    TTF_Font* font; // we'll use this for showing text in the controls screen -c
    int which;
    int successfulJobs;
  public:
    MyGame(int level=1):Game(){
        which = 0;
        successfulJobs = 0;
        //--- c
        // load a font for the controls screen - c
        font = TTF_OpenFont("./Fonts/BungeeSpice-Regular.ttf",28);
        if (!font) 
        {
          cerr << "Failed to load font: " << TTF_GetError() << endl;
          }
          // --- c end
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

        ifstream bin("./game_textFiles/loadbackground.txt");
        while(!bin.eof()) {
          bin>>tool;
          background.push_back(new Tile(getMM(),tool));
        }
        bin.close();
        
        wave = new Sound("./Sounds/footsteo.wav");
        grab = new Sound("./Sounds/grab.wav");
        text = new Text(getRen(),"PRESS ENTER",45,450,300,false);
        help = new Text(getRen(),"please help me my computer is overheating",12,460,150);// 
        dt=.01;
        // Load and play background music -c
        music = Mix_LoadMUS("./Sounds/Green Meadows.ogg");
        if (!music) {
          cerr << "Failed to load music: " << Mix_GetError() << endl;
          } else {
            Mix_PlayMusic(music, -1); // -1 = loop forever
            Mix_VolumeMusic(MIX_MAX_VOLUME / 2); // Play at 50% volume
            //I researched how to use SDL_mixer and Mix_Music through tutorials on yt soo
            // mb if there is a easier way - c
      }   
    }

  void loop() {
    for (auto tool : tools)
      tool->loop(dt);
    for (auto tool : characters)
      tool->loop(dt);

    if (which == 0) {
      background[0]->render(getRen());
      text->display();
    } else if (which == 1) {
      background[1]->render(getRen());
      for (int i = 0; i < 3; i++)
        characters[i]->render(getRen());
    } else if (which == 2) {
      background[3]->render(getRen());
      tools[5]->render(getRen());
      help->display();
    } else if (which == 3) {
      background[2]->render(getRen());
      for (int i = 0; i < 3; i++)
        if (characters[i]->isActive())
          characters[i]->render(getRen());
      tools[6]->setActive(false);
    } else {
      tools[6]->setActive(true);
      background[4]->render(getRen());
      for (int i = 0; i < 5; i++)
        if (tools[i]->isActive())
          tools[i]->render(getRen());
      tools[6]->render(getRen());
    }
    
      SDL_RenderPresent(getRen());
      SDL_Delay(1000.0 * dt);
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


            if (event.key.keysym.sym==SDLK_m) {
            MotherboardMinigame mb(getRen(), getMM(), successfulJobs);
            mb.run();
            }
            // ---- c
            if (event.key.keysym.sym == SDLK_h) 
            {
              // show the controls/help screen when H is pressed
              showControlsScreen(getRen(), font);
              } // ---c end
            if (event.key.keysym.sym==SDLK_ESCAPE) running=false;

            if (event.key.keysym.sym==SDLK_SPACE) {
              for(int i = 0;i<5;i++){
              if(tools[i]->isActive()) {
                  tools[i]->setActive(!tools[6]->isTouching(*tools[i]));
                  grab->play();
              }
              }
            }

           if (event.key.keysym.sym==SDLK_w) {if(tools[6]->isActive()) tools[6]->sety(-10); 
            else {
             for(int i = 0;i<3;i++)if(characters[i]->isActive()) characters[i]->sety(-10);
             wave->play();
            }
          }
          if (event.key.keysym.sym == SDLK_s) {
          int character = 0;

          if (tools[6]->isActive())
            tools[6]->sety(10);
          else {

            if (characters[character]->isActive()) {
              characters[character]->sety(10);
              characters[character]->walkingVertical(true);
            }
            wave->play();
          }
        }
        if (event.key.keysym.sym == SDLK_a) {
          int character = 0;

          if (tools[6]->isActive()) {
            tools[6]->setx(-10);
          } else {

            if (characters[character]->isActive()) {
              characters[character]->setx(-10);
              characters[character]->setDirection(true); // face left
            }
            wave->play();
          }
        }

        if (event.key.keysym.sym == SDLK_d) {
          int character = 0;

          if (tools[6]->isActive()) {
            tools[6]->setx(10);
          } else {

            if (characters[character]->isActive()) {
              characters[character]->setx(10);
              characters[character]->setDirection(false); // face right
            }
            wave->play();
          }
        }
            

        //**********************************  Moving character 1
        //*****************************  */
        if (event.key.keysym.sym == SDLK_UP) {
          int character = 1;
          if (tools[6]->isActive())
            tools[6]->sety(-10);
          else {

            if (characters[character]->isActive()) {
              characters[character]->sety(-10);
              characters[character]->walkingVertical(true);
            }
            wave->play();
          }
        }
        if (event.key.keysym.sym == SDLK_DOWN) {
          int character = 1;
          if (tools[6]->isActive())
            tools[6]->sety(10);
          else {
            if (characters[character]->isActive()) {
              characters[character]->sety(10);
              characters[character]->walkingVertical(true);
            }
            wave->play();
          }
        }
        if (event.key.keysym.sym == SDLK_LEFT) {
          int character = 1;
          if (tools[6]->isActive()) {
            tools[6]->setx(-10);
          } else {

            if (characters[character]->isActive()) {
              characters[character]->setx(-10);
              characters[character]->setDirection(true); // face left
            }
            wave->play();
          }
        }

        if (event.key.keysym.sym == SDLK_RIGHT) {
          int character = 1;
          if (tools[6]->isActive()) {
            tools[6]->setx(10);
          } else {

            if (characters[character]->isActive()) {
              characters[character]->setx(10);
              characters[character]->setDirection(false); // face right
              wave->play();
          }
        }
      }
    }

      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE)
          running = false;
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
      // - c
      Mix_HaltMusic();       // Stop any playing music
      Mix_FreeMusic(music);  // Clean up
      TTF_CloseFont(font); // free the font when the game ends
      // - c end
    }

};

int main(void) {
  MyGame game(1);
  game.run();
  return 0;
}
