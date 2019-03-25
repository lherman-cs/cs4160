#include "player/human.h"

Human::Human(const GameScreen* const game, std::string name)
    : Player(game, name, 0), onType(false) {}
Human::~Human() {}

void Human::decide(const Uint8* const keystate, int& quantity, int& type) {
  // Begin Game
  if (keystate[SDL_SCANCODE_RETURN]) {
    emit<GameEvent::Done>(GameEvent::Done());
    // Reset onType for future UI interaction
    onType = false;
    return;
  }

  // Selection
  if (keystate[SDL_SCANCODE_W])
    onType ? type-- : quantity++;

  else if (keystate[SDL_SCANCODE_S])
    onType ? type++ : quantity++;

  else if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D])
    onType ^= 1;
}