#include "screens/game/human.h"

Human::Human(const GameScreen* const game, Dice d, std::string name)
    : Player(game, d, name, 0), onFace(false) {}
Human::~Human() {}

bool Human::decide(const Uint8* const keystate, std::shared_ptr<Bet> bet) {
  // Stop the human's turn if they hit enter
  if (keystate[SDL_SCANCODE_RETURN] || keystate[SDL_SCANCODE_B]) {
    bet->submit();
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

bool Human::callLiar(const Uint8* const keystate,
                     const std::shared_ptr<const Bet> bet) {
  (void)bet;
  if (keystate[SDL_SCANCODE_L]) return true;
  return false;
}