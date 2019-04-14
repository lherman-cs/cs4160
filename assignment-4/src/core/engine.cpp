#include "core/engine.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include "core/promise.h"
#include "global/gameData.h"
#include "global/mixer.h"
#include "global/navigator.h"
#include "screens/intro.h"
#include "sprite/multisprite.h"
#include "sprite/sprite.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Engine::~Engine() { std::cout << "Terminating program" << std::endl; }

Engine::Engine()
    : rc(RenderContext::getInstance()),
      mixer(Mixer::getInstance()),
      clock(Clock::getInstance()),
      navigator(Navigator::getInstance()),
      promiseScheduler(PromiseScheduler::getInstance()),
      loading(Loading::getInstance()),
      renderer(rc.getRenderer()) {
  navigator.push<IntroScreen>();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  navigator.getCurrentScreen()->draw();
  loading.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  navigator.getCurrentScreen()->update(ticks);
  loading.update(ticks);
}

void Engine::forward(bool &done) {
  static SDL_Event event;
  static const Uint8 *keystate;
  static Uint32 ticks = clock.getElapsedTicks();

  // The next loop polls for events, guarding against key bounce:
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      done = true;
      return;
    }

    if (event.type == SDL_KEYDOWN) {
      keystate = SDL_GetKeyboardState(NULL);
      navigator.getCurrentScreen()->onKeyDown(keystate);

      if (keystate[SDL_SCANCODE_R]) {
        navigator.reset();
        navigator.push<IntroScreen>();
        return;
      }

      if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
        done = true;
        return;
      }
      if (keystate[SDL_SCANCODE_Z]) {
        if (clock.isPaused())
          clock.unpause();
        else
          clock.pause();
      }
    }
  }

  // In this section of the event loop we allow key bounce:
  ticks = clock.getElapsedTicks();
  if (ticks > 0) {
    clock.incrFrame();
    draw();
    update(ticks);
    promiseScheduler.update(ticks);
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

void Engine::play() {
  mixer.background.play();
  emscripten_set_main_loop_arg(loop, (void *)this, 60, 1);
}

#else
void Engine::play() {
  bool done = false;
  mixer.background.play();
  while (!done) {
    forward(done);
  }
}
#endif
