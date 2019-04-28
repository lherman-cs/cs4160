// command:game-bet
// command:game-call-liar
// command:game-leave

// server handles dice, which one is deleted, what they are
// server updates game state whenever xxx
//      maybe turn update? & call liar?

// server calls
//      die (or similar) (this closes the client, prints a message and turns you
//        into a bot)(player passively watches but is a bot?)
//      update (this gives appropariate state info to the game)

// decide does read/send connection
// player will have state machine
// states:
//      decide-ing (not you)
//      decide-ing (you)
//      verifying call liar

#pragma once

#include "global/global.h"   // navigatior and others
#include "net/state.h"       // holding game state
#include "net/tcp.h"         // networking
#include "screens/screen.h"  // inheritance
#include "util/ioMod.h"      // drawing text to screen
#include "util/world.h"      // drawing background

class Player;
class Bet;

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity

class NetGameScreen : public Screen {
 public:
  NetGameScreen(int difficulty);
  ~NetGameScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  // Networking
  std::shared_ptr<TCP> session;
  const std::string id;

  // Game Data
  enum Status { Initalizing, Ongoing, CallingLiar, Finish, TurnTransition };
  int index;
  State gameData;
  Status state{Initalizing};

  // Drawing
  IoMod menuWriter{60};
  SDL_Color secondaryColor{182, 148, 103, 255};
  IoMod loadingWriter{32};
  Navigator& navigator{Global::get().navigator};
  World background{"screens/game/background"};

  // Callbacks and State Management
  void onDone();
  void onCallLiar(std::shared_ptr<Player> caller);
  void removeLosers();
};