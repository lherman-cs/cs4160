#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "bet.h"
#include "core/interface.h"
#include "dice.h"
#include "game.h"

/* ignore effc++ since faces is just going to be used */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class GameScreen;
class Bet;
class Player : public Drawable {
 public:
  virtual ~Player();
  const std::string name;
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
  virtual bool callLiar(const std::shared_ptr<const Bet> bet) { return false; }
  virtual bool callLiar(const Uint8 *const keystate,
                        const std::shared_ptr<const Bet> bet) {
    (void)keystate;
    return false;
  }

 protected:
  Player(const GameScreen *const game, Dice dice, const std::string &name,
         uint type);
  const GameScreen *const game;
  Dice dice;

  friend class GameScreen;
};

#pragma GCC diagnostic pop