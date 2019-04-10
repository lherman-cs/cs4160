#include "screens/game/human.h"

Human::Human(const GameScreen* const game, std::string name)
    : Player(game, name, 0), onFace(false) {}
Human::~Human() {}

bool Human::decide(const Uint8* const keystate, std::shared_ptr<Bet> bet) {
  // Stop the human's turn if they hit enter
  if (keystate[SDL_SCANCODE_RETURN]) {
    onFace = false;
    return true;
  }

  // Selection
  if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])
    onFace ? bet->increment(Bet::Face) : bet->increment(Bet::Quantity);

  else if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
    onFace ? bet->decrement(Bet::Face) : bet->decrement(Bet::Quantity);

  else if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D] ||
           keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT]) {
    onFace = !onFace;
    bet->select(onFace ? Bet::Face : Bet::Quantity);
  }

  return false;
}