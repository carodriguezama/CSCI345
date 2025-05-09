#pragma once
#include "SDL.h"

#include "Tile.h"
#include <vector>

class ServiceTable {
public:
  ServiceTable(SDL_Renderer *renderer, MediaManager *mm, int &whichRef);
  ~ServiceTable();

  void render();
  void update(float dt);
  void handleEvent(SDL_Event &event);
  bool isTouching(Tile *a, Tile *b);
  bool isTouching(SDL_Rect a, SDL_Rect b);
  void renderScrew(SDL_Rect pos);
  void renderImage(const std::string &path);
  SDL_Rect getScrewdriverTipRect();

private:
  SDL_Renderer *ren;
  MediaManager *mm;
  Tile *background;
  Tile *screwdriver;
  Tile *brokenComputer;
  Tile *hand;
  Tile *halfComputer;
  Tile *fixedComputer;
  Tile *screw;
  int &which;

  bool screwdriverActive = true;
  bool holdingScrewdriver = false;
  enum MiniGamePhase {
    PHASE_REMOVE_SCREWS,
    PHASE_SCREEN_OFF,
    PHASE_FIXED,
    PHASE_REATTACH_SCREWS,
    PHASE_DONE
  };

  MiniGamePhase currentPhase = PHASE_REMOVE_SCREWS;

  bool screw1Removed = false;
  bool screw2Removed = false;
  bool screw1Reattached = false;
  bool screw2Reattached = false;

  SDL_Rect screw1Pos = {500, 120, 32, 32}; // adjust these
  SDL_Rect screw2Pos = {600, 120, 32, 32};

  Uint32 phaseTimer = 0;
};
