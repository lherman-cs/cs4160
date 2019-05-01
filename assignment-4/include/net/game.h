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

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity

class NetGameScreen : public Screen {
 public:
  NetGameScreen(std::shared_ptr<TCP> session, int index, int difficulty);
  ~NetGameScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  enum Status { Initializing, Ongoing, OnCall, OnFinish, OnRoll };

  // Networking
  std::shared_ptr<TCP> session;

  // Game Data
  unsigned int index;
  State gameData{};
  Status state{Status::Initializing};

  // Drawing
  IoMod menuWriter{36};
  IoMod helperWriter{36};
  IoMod listWriter{18};
  SDL_Color normalColor{255, 255, 255, 255};
  SDL_Color hoverColor{255, 255, 0, 255};
  SDL_Color secondaryColor{186, 170, 77, 255};
  Die helperDie{Vector2f(525, 408), Die::State::visible};
  IoMod loadingWriter{32};
  Navigator& navigator{Global::get().navigator};
  World background{"screens/game/background"};
  std::shared_ptr<Widget> callButton{nullptr};

  // Callbacks and State Management
  void onDone();
  void onCallLiar(std::shared_ptr<Player> caller);
  void removeLosers();
};