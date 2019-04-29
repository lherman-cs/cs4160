#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "bet.h"
#include "core/interface.h"
#include "dice.h"
#include "net/state.h"

/* ignore effc++ since faces is just going to be used */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class State;
class Bet;

class Player : public Drawable {
 public:
  virtual ~Player();
  std::string name;
  const uint type;
  virtual void draw() const;
  virtual bool decide(Uint32 ticks, std::shared_ptr<Bet> bet) {
    (void)ticks;
    (void)bet;
    return false;
  };
  virtual bool decide(const Uint8 *const keystate, std::shared_ptr<Bet> bet) {
    (void)keystate;
    (void)bet;
    return false;
  };
  virtual bool callLiar(const std::shared_ptr<const Bet> bet) {
    (void)bet;
    return false;
  }
  virtual bool callLiar(const Uint8 *const keystate,
                        const std::shared_ptr<const Bet> bet) {
    (void)keystate;
    (void)bet;
    return false;
  }

 protected:
  Player(const State *const state, Dice dice, const std::string &name,
         uint type);
  Dice dice;
  const State *state;

  friend class GameScreen;
  friend class NetGameScreen;
};

#pragma GCC diagnostic pop