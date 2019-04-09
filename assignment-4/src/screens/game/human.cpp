#include "screens/game/human.h"

Human::Human(const GameScreen* const game, std::string name)
    : Player(game, name, 0), onType(false) {}
Human::~Human() {}

bool Human::decide(const Uint8* const keystate, std::shared_ptr<Bet> bet) {
  // Stop the human's turn if they hit enter
  if (keystate[SDL_SCANCODE_RETURN]) {
    onType = false;
    return true;
  }

  // Selection
  if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])
    onType ? bet->increment(Bet::Face) : bet->increment(Bet::Quantity);

  else if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
    onType ? bet->decrement(Bet::Face) : bet->decrement(Bet::Quantity);

  else if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D] ||
           keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT])
    onType ^= 1;

  return false;
}