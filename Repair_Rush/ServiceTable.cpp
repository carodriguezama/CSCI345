#include "ServiceTable.h"
#include "SDL.h"
#include <iostream>
using namespace std;

ServiceTable::ServiceTable(SDL_Renderer *renderer, MediaManager *mm,
                           int &whichRef)
    : ren(renderer), mm(mm), which(whichRef) {
  this->ren = renderer;
  this->mm = mm;
  cout << "Creating Tile for mini-game background...\n";
  background = new Tile(mm, "Images/Background/serviceTable.png");
  hand = new Tile(mm, "Images/Tools/hand2.bmp");
  screwdriver = new Tile(mm, "Images/Tools/ScrewDriver.bmp");
  brokenComputer = new Tile(mm, "Images/Tools/brokenHingeComputer.png");
  halfComputer = new Tile(mm, "Images/Tools/noScreenComputer.png");
  fixedComputer = new Tile(mm, "Images/Tools/fixedHingeComputer.png");
  screw = new Tile(mm, "Images/Tools/screw.png");

  // Set broken computer size and position
  brokenComputer->rect.x = 40;
  brokenComputer->rect.y = 20;
  brokenComputer->rect.w = 700;
  brokenComputer->rect.h = 400;

  halfComputer->rect = brokenComputer->rect;
  fixedComputer->rect = brokenComputer->rect;

  // Screw positions (example: lower corners of screen or wherever you need
  // them)
  screw1Pos = {220, 198, 32, 32}; // left screw moved in + down
  screw2Pos = {540, 198, 32, 32}; // right screw moved in + down

  screwdriver->rect.x = 610;
  screwdriver->rect.y = 70;

  hand->rect.x = 600;
  hand->rect.y = 300;
}

void ServiceTable::render() {
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);

  SDL_GetRendererOutputSize(ren, &background->rect.w, &background->rect.h);
  background->rect.x = 0;
  background->rect.y = 0;
  background->render(ren); // Always first

  // Computer state image depending on phase
  if (currentPhase == PHASE_REMOVE_SCREWS) {
    brokenComputer->render(ren);
  } else if (currentPhase == PHASE_SCREEN_OFF) {
    halfComputer->render(ren);
  } else if (currentPhase == PHASE_FIXED ||
             currentPhase == PHASE_REATTACH_SCREWS ||
             currentPhase == PHASE_DONE) {
    fixedComputer->render(ren);
  }

  // Screws only show in certain phases
  if (currentPhase == PHASE_REMOVE_SCREWS) {
    if (!screw1Removed)
      renderScrew(screw1Pos);
    if (!screw2Removed)
      renderScrew(screw2Pos);
  } else if (currentPhase == PHASE_REATTACH_SCREWS) {
    if (!screw1Reattached)
      renderScrew(screw1Pos);
    if (!screw2Reattached)
      renderScrew(screw2Pos);
  }

  // Tools and hand (always on top)
  if (screwdriverActive || holdingScrewdriver)
    screwdriver->render(ren, 180.0);

  hand->render(ren);
}

void ServiceTable::renderImage(const string &path) {
  SDL_Texture *tex = mm->get(path);
  if (tex) {
    SDL_Rect dest = {0, 0, 800, 600}; // full screen or adjust to match table
    SDL_RenderCopy(ren, tex, NULL, &dest);
  } else {
    cerr << "Failed to load image: " << path << endl;
  }
}

bool ServiceTable::isTouching(Tile *a, Tile *b) {
  return a->rect.x < b->rect.x + b->rect.w &&
         a->rect.x + a->rect.w > b->rect.x &&
         a->rect.y < b->rect.y + b->rect.h && a->rect.y + a->rect.h > b->rect.y;
}
bool ServiceTable::isTouching(SDL_Rect a, SDL_Rect b) {
  return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h &&
         a.y + a.h > b.y;
}

void ServiceTable::renderScrew(SDL_Rect pos) {
  screw->rect = pos;
  screw->render(ren);
}
SDL_Rect ServiceTable::getScrewdriverTipRect() {
  SDL_Rect tip;
  tip.w = 30;
  tip.h = 30;

  // Place tip at the bottom-center of the screwdriver (since it's upside down)
  tip.x = screwdriver->rect.x + (screwdriver->rect.w / 2) - (tip.w / 2);
  tip.y = screwdriver->rect.y + screwdriver->rect.h - tip.h;

  return tip;
}

void ServiceTable::handleEvent(SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {

    switch (event.key.keysym.sym) {
    case SDLK_w:
      hand->rect.y -= 22;
      if (holdingScrewdriver)
        screwdriver->rect.y -= 22;
      break;

    case SDLK_s:
      hand->rect.y += 22;
      if (holdingScrewdriver)
        screwdriver->rect.y += 22;
      break;

    case SDLK_a:
      hand->rect.x -= 22;
      if (holdingScrewdriver)
        screwdriver->rect.x -= 22;
      break;

    case SDLK_d:
      hand->rect.x += 22;
      if (holdingScrewdriver)
        screwdriver->rect.x += 22;
      break;

    case SDLK_SPACE:
      if (screwdriverActive && isTouching(hand, screwdriver)) {
        screwdriverActive = false;
        holdingScrewdriver = true;
      }
      break;
    }
  }
  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
    // Try to pick up screwdriver
    if (screwdriverActive && isTouching(hand, screwdriver)) {
      screwdriverActive = false;
      holdingScrewdriver = true;
      cout << "Picked up screwdriver\n";
      return;
    }

    if (!holdingScrewdriver) {
      cout << "Not holding screwdriver\n";
      return;
    }

    cout << "Holding screwdriver — trying to unscrew...\n";

    // 🛠 Use hand->rect instead of screwdriver tip
    SDL_Rect toolHitbox = screwdriver->rect;

    if (currentPhase == PHASE_REMOVE_SCREWS) {
      if (!screw1Removed && isTouching(hand->rect, screw1Pos)) {
        screw1Removed = true;
        cout << "Screw 1 removed\n";
      }

      if (!screw2Removed && isTouching(hand->rect, screw2Pos)) {
        screw2Removed = true;
        cout << "Screw 2 removed\n";
      }

      // Only advance to screen-off if BOTH screws are removed
      if (screw1Removed && screw2Removed &&
          currentPhase == PHASE_REMOVE_SCREWS) {
        cout << "Both screws removed. Advancing...\n";
        currentPhase = PHASE_SCREEN_OFF;
        phaseTimer = SDL_GetTicks();
      }

    }

    else if (currentPhase == PHASE_REATTACH_SCREWS) {
      if (!screw1Reattached && isTouching(toolHitbox, screw1Pos)) {
        screw1Reattached = true;
      }
      if (!screw2Reattached && isTouching(toolHitbox, screw2Pos)) {
        screw2Reattached = true;
      }

      if (screw1Reattached && screw2Reattached) {
        currentPhase = PHASE_DONE;
      }
    }
  }
  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
    currentPhase = PHASE_DONE; // optional: reset state
    which = 3;                 // or however you're signaling exit
    return;
  }
}

void ServiceTable::update(float dt) {

  if (currentPhase == PHASE_SCREEN_OFF && SDL_GetTicks() - phaseTimer > 2000) {
    currentPhase = PHASE_FIXED;
    phaseTimer = SDL_GetTicks();
  } else if (currentPhase == PHASE_FIXED &&
             SDL_GetTicks() - phaseTimer > 2000) {
    currentPhase = PHASE_REATTACH_SCREWS;
  }
}

ServiceTable::~ServiceTable() {
  delete background;
  delete hand;
  delete screwdriver;
  delete brokenComputer;
  delete halfComputer;
  delete fixedComputer;
  delete screw;
}
