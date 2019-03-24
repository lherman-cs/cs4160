#include "screens/game.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include "player/bot.h"
#include "screens/help.h"
#include "util/ioMod.h"

using namespace std::placeholders;

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity

GameScreen::GameScreen(int players, int bots, int difficulty)
    : background("game-background"),
      difficulty(difficulty),
      turn(1),
      players(players),
      menuWriter(70, {88, 53, 31, 255}),
      navigator(Navigator::getInstance()) {}

GameScreen::~GameScreen() {}

void GameScreen::onKeyDown(const Uint8* const keystate) {}

void GameScreen::draw() const { background.draw(); }

void GameScreen::update(Uint32 ticks) { (void)ticks; }