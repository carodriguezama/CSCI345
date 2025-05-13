#include "Animation.h"
#include "ControlScreen.h"
#include "Game.h"
#include "SDL.h"
#include "ServiceTable.h"
#include "Sound.h"
#include "Sprite.h"
#include "MotherboardMinigame.h"
#include "Text.h"
#include "SolderingGame.h"
#include "GameOver.h"
#include "Tile.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;
Mix_Music *music = nullptr;

class MyGame : public Game {
  float dt;
  int check, xpos, ypos;
  vector<Sprite *> tools;
  vector<Sprite *> characters;
  vector<Tile *> background;
  Sound *wave, *grab;
  Text *text, *help;
  string tool;
  bool start;
  TTF_Font *font;
  int which;
  int successfulJobs = 0; // checking 

  ServiceTable *minigame;

public:
  MyGame(int level = 1) : Game() {
    which = 0;

    font = TTF_OpenFont("./Fonts/BungeeSpice-Regular.ttf", 28);
    if (!font) {
      cerr << "Failed to load font: " << TTF_GetError() << endl;
    }

    start = true;

    ifstream in("./game_textFiles/loadtools.txt");
    while (!in.eof()) {
      in >> tool >> xpos >> ypos;
      tools.push_back(new Sprite(getMM(), tool, xpos, ypos));
    }
    in.close();

    ifstream cin("./game_textFiles/Character.txt");
    while (!cin.eof()) {
      cin >> tool >> xpos >> ypos;
      characters.push_back(new Sprite(getMM(), tool, xpos, ypos));
    }
    cin.close();

    ifstream bin("./game_textFiles/loadbackground.txt");
    while (!bin.eof()) {
      bin >> tool;
      background.push_back(new Tile(getMM(), tool));
    }
    bin.close();

    // ADD: mini-game background
    background.push_back(new Tile(
        getMM(), "Images/Background/serviceTable.png")); // background[5]
    minigame = new ServiceTable(getRen(), getMM(), which, successfulJobs);
    wave = new Sound("./Sounds/footsteo.wav");
    grab = new Sound("./Sounds/grab.wav");
    text = new Text(getRen(), "PRESS ENTER", 45, 450, 300, false);
    help = new Text(getRen(), "please help me my computer is overheating", 12,
                    460, 150);
    dt = .01;

    music = Mix_LoadMUS("./Sounds/Green Meadows.ogg");
    if (!music) {
      cerr << "Failed to load music: " << Mix_GetError() << endl;
    } else {
      Mix_PlayMusic(music, -1);
      Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    }
  }
  void selectCharacter(int index) {
    for (int i = 0; i < characters.size(); i++)
      characters[i]->setActive(i == index);
    which = 2;
  }

  void loop() {
    SDL_Rect tableHitbox = {400, 300, 120, 80};

    int charX = characters[0]->getX();
    int charY = characters[0]->getY();

    bool nearTable = (charX + 20 > tableHitbox.x &&
                      charX < tableHitbox.x + tableHitbox.w + 20 &&
                      charY + 20 > tableHitbox.y &&
                      charY < tableHitbox.y + tableHitbox.h + 20);

    for (auto tool : tools)
      tool->loop(dt);
    for (auto c : characters)
      if (c->isActive())
        c->loop(dt);

    if (which == 0) {
      background[0]->render(getRen());
      text->display();
    } else if (which == 1) {
      background[1]->render(getRen());
      for (int i = 0; i < 3; i++)
        if (characters[i]->isActive())
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
    } else if (which == 5) {
      SDL_Event event;

      if (SDL_PollEvent(&event)) {
        minigame->handleEvent(event);

        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_CLOSE)
          running = false;
      }

      minigame->update(dt);
      minigame->render();

      SDL_RenderPresent(getRen());
      SDL_Delay(1000.0 * dt);
      return;

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

    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_1)
          selectCharacter(0);
        if (event.key.keysym.sym == SDLK_2)
          selectCharacter(1);
        if (event.key.keysym.sym == SDLK_3)
          selectCharacter(2);

        if (event.key.keysym.sym == SDLK_RETURN) {
          for (auto c : characters)
            c->setActive(
                false); // <- DISABLE all characters for selection screen
          which = 1;
        }

        if (event.key.keysym.sym == SDLK_r)
          which = 3;
        if (event.key.keysym.sym == SDLK_b)
          which = 2;
        if (event.key.keysym.sym == SDLK_c)
          which = 4;

        if (event.key.keysym.sym==SDLK_m) 
        {
          MotherboardMinigame mb(getRen(), getMM(), successfulJobs);
          mb.run();
        }
        if (event.type == SDL_KEYDOWN) 
        {
          if (event.key.keysym.sym == SDLK_e) 
          {
            SolderingGame sd(getRen(), getMM(), successfulJobs);
            bool gameSuccess = sd.run();
            if (gameSuccess && successfulJobs >= 2) 
            {
                showGameOverScreen(getRen(), font);
            }

          } 
        }
        
        if (event.key.keysym.sym == SDLK_h) {
          showControlsScreen(getRen(), font);
        }
        if (event.key.keysym.sym == SDLK_ESCAPE)
          running = false;

        if (event.key.keysym.sym == SDLK_SPACE) {
          for (int i = 0; i < 5; i++) {
            if (tools[i]->isActive()) {
              tools[i]->setActive(!tools[6]->isTouching(*tools[i]));
              grab->play();
            }
          }
        }

        if (event.key.keysym.sym == SDLK_w) {
          if (tools[6]->isActive())
            tools[6]->sety(-10);
          else {
            for (int i = 0; i < 3; i++)
              if (characters[i]->isActive())
                characters[i]->sety(-10);
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
              characters[character]->setDirection(true);
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
              characters[character]->setDirection(false);
            }
            wave->play();
          }
        }

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
              characters[character]->setDirection(true);
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
              characters[character]->setDirection(false);
              wave->play();
            }
          }
        }

        // ENTER MINI-GAME
        if (event.key.keysym.sym == SDLK_i) 
        {
          if (which != 5) 
          {
            which = 5;
          }
        }
      }

      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE)
          running = false;
      }
    }
  }

  ~MyGame() {
    wave->free();
    grab->free();
    for (auto tool : tools)
      delete tool;
    for (auto tool : characters)
      delete tool;
    for (auto tool : background)
      delete tool;
    text->destroy();
    help->destroy();
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    TTF_CloseFont(font);
    delete minigame;
  }
};

int main(void) 
{
  MyGame game(1);
  game.run();
  return 0;
}
