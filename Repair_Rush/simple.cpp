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
#include "Tile.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

class MyGame : public Game {
  float dt;
  int check, xpos, ypos;
  vector<Sprite *> tools;
  vector<Sprite *> characters;
  vector<Tile *> background;
  vector<Sound*> sounds;
  Text *text, *help;
  string tool,chan;
  bool start;
  TTF_Font *font;
  int which;
  int successfulJobs;

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

    ifstream ain("./game_textFiles/audio.txt");
    while (!ain.eof()) {
      ain >> tool; //>> chan >> frame;
      sounds.push_back(new Sound(tool));
    }
    ain.close();

    sounds[2]->loop();

    // ADD: mini-game background
    background.push_back(new Tile(
        getMM(), "Images/Background/serviceTable.png")); // background[5]
    minigame = new ServiceTable(getRen(), getMM(), which);

    text = new Text(getRen(), "PRESS ENTER", 45, 450, 300, false);
    help = new Text(getRen(), "please help me my computer is overheating", 12,
                    460, 150);
    dt = .01;
  }
  void selectCharacter(int index) {
    for (int i = 0; i < characters.size(); i++)
      characters[i]->setActive(i == index);
    which = 2;
  }
  
  void handleMovement(SDL_Keycode key) {
    int character = (key >= SDLK_UP && key <= SDLK_RIGHT) ? 1 : 0;

    int dx = 0, dy = 0;
    bool vertical = false, direction = false;

    switch (key) {
        case SDLK_w: dy = -10; vertical = true; break;
        case SDLK_s: dy = 10;  vertical = true; break;
        case SDLK_a: dx = -10; direction = true; break;
        case SDLK_d: dx = 10;  direction = false; break;

        case SDLK_UP:    dy = -10; vertical = true; break;
        case SDLK_DOWN:  dy = 10;  vertical = true; break;
        case SDLK_LEFT:  dx = -10; direction = true; break;
        case SDLK_RIGHT: dx = 10;  direction = false; break;
    }

    if (tools[6]->isActive()) {
        if (dx != 0) tools[6]->setx(dx);
        if (dy != 0) tools[6]->sety(dy);
    } else {
        if (characters[character]->isActive()) {
            if (dx != 0) {
                characters[character]->setx(dx);
                characters[character]->setDirection(direction);
            }
            if (dy != 0) {
                characters[character]->sety(dy);
                characters[character]->walkingVertical(vertical);
            }
            sounds[0]->play();
        }
    }
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
      SDL_Keycode key = event.key.keysym.sym;

      switch (key) {
          case SDLK_1: selectCharacter(0); break;
          case SDLK_2: selectCharacter(1); break;
          case SDLK_3: selectCharacter(2); break;

          case SDLK_RETURN:
              for (auto c : characters) c->setActive(false);
              which = 1;
              break;

          case SDLK_r: which = 3; break;
          case SDLK_b: which = 2; break;
          case SDLK_c: which = 4; break;
          case SDLK_i: if (which != 5) which = 5; break;

          case SDLK_m: {
              MotherboardMinigame mb(getRen(), getMM(), successfulJobs);
              mb.run();
              break;
          }

          case SDLK_e: {
              SolderingGame sd(getRen(), getMM(), successfulJobs);
              sd.run();
              break;
          }

          case SDLK_h:
              showControlsScreen(getRen(), font);
              break;

          case SDLK_ESCAPE:
              running = false;
              break;

          case SDLK_SPACE:
              for (int i = 0; i < 5; ++i) {
                  if (tools[i]->isActive()) {
                      tools[i]->setActive(!tools[6]->isTouching(*tools[i]));
                      sounds[1]->play();
                  }
              }
              break;

          case SDLK_w:
          case SDLK_s:
          case SDLK_a:
          case SDLK_d:
          case SDLK_UP:
          case SDLK_DOWN:
          case SDLK_LEFT:
          case SDLK_RIGHT:
              handleMovement(key);
              break;

          default:
              break;
      }
    }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE)
          running = false;
      }
    }
  }

  ~MyGame() {
    for (int i = 0; i < sounds.size();i++) {
      sounds[i]->free();
      delete sounds[i];
    }

    for (auto tool : tools)
      delete tool;
    for (auto tool : characters)
      delete tool;
    for (auto tool : background)
      delete tool;
    text->destroy();
    help->destroy();
    TTF_CloseFont(font);
    delete minigame;
  }
};

int main(int argc, char* argv[]) {
  MyGame game(1);
  game.run();
  return 0;
}
