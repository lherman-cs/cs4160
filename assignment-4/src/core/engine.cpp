#include "core/engine.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include "global/gameData.h"
#include "sprite/multisprite.h"
#include "sprite/sprite.h"
#include "sprite/twoWayMultisprite.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Engine::~Engine() {
  for (auto &sprite : sprites) delete sprite;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine()
    : rc(RenderContext::getInstance()),
      io(IoMod::getInstance()),
      clock(Clock::getInstance()),
      renderer(rc.getRenderer()),
      background("background",
                 Gamedata::getInstance().getXmlInt("background/factor")),
      waitress("waitress",
               Gamedata::getInstance().getXmlInt("waitress/factor")),
      hangingLightBulb("hangingLightBulb", Gamedata::getInstance().getXmlInt(
                                               "hangingLightBulb/factor")),
      viewport(Viewport::getInstance()),
      sprites(),
      currentSprite(0),
      makeVideo(false) {
  const auto pacman = new TwoWayMultiSprite("Pacman");
  pacman->setScale(2);

  auto i = 1;
  auto id = "Ramens/ramen-" + std::to_string(i);
  while (Gamedata::getInstance().checkTag(id + "/file")) {
    const auto ramen = new MultiSprite(id);
    ramen->setScale(1.5);
    sprites.emplace_back(ramen);
    i++;
    id = "Ramens/ramen-" + std::to_string(i);
  }

  sprites.emplace_back(pacman);
  Viewport::getInstance().setObjectToTrack(pacman);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  background.draw();
  waitress.draw();
  hangingLightBulb.draw();

  for (const auto &sprite : sprites) sprite->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for (const auto &sprite : sprites) sprite->update(ticks);

  background.update();
  waitress.update();
  hangingLightBulb.update();
  viewport.update();  // always update viewport last
}

void Engine::switchSprite() {
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::forward(bool &done) {
  static SDL_Event event;
  static const Uint8 *keystate;
  static Uint32 ticks = clock.getElapsedTicks();

  // The next loop polls for events, guarding against key bounce:
  while (SDL_PollEvent(&event)) {
    keystate = SDL_GetKeyboardState(NULL);
    if (event.type == SDL_QUIT) {
      done = true;
      break;
    }
    if (event.type == SDL_KEYDOWN) {
      if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
        done = true;
        break;
      }
      if (keystate[SDL_SCANCODE_P]) {
        if (clock.isPaused())
          clock.unpause();
        else
          clock.pause();
      }
      if (keystate[SDL_SCANCODE_T]) {
        switchSprite();
      }
    }
  }

  // In this section of the event loop we allow key bounce:
  ticks = clock.getElapsedTicks();
  if (ticks > 0) {
    clock.incrFrame();
    draw();
    update(ticks);
  }
}

#ifdef __EMSCRIPTEN__

void loop(void *data) {
  Engine *self = (Engine *)data;
  bool done = false;
  self->forward(done);
  if (done) {
    emscripten_cancel_main_loop();
  }
}

void Engine::play() { emscripten_set_main_loop_arg(loop, (void *)this, 60, 1); }

#else
void Engine::play() {
  bool done = false;
  while (!done) {
    forward(done);
  }
}
#endif