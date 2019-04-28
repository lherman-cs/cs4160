#include "net/game.h"
// #include <algorithm>
// #include <functional>
#include <iostream>
// #include <sstream>
// #include "global/global.h"
// #include "screens/game/bot.h"
#include "net/encoder.h"
#include "net/player.h"
#include "screens/game/dice.h"
#include "screens/game/human.h"
// #include "screens/help.h"
// #include "screens/intro.h"
#include "util/ioMod.h"
#include "widget/loading.h"

using namespace std::placeholders;

// game screen
//  accepts face and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of face and quantity

NetGameScreen::NetGameScreen(int difficulty) {
  (void)difficulty;
  auto gameSession = std::make_shared<TCP>();
}

NetGameScreen::~NetGameScreen() {}