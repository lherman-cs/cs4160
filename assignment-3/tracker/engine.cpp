#include "engine.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include "frameGenerator.h"
#include "gameData.h"
#include "multisprite.h"
#include "sprite.h"
#include "twoWayMultisprite.h"

Engine::~Engine() {
  for (auto &sprite : sprites) delete sprite;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine()
    : rc(RenderContext::getInstance()),
      io(IoMod::getInstance()),
      clock(Clock::getInstance()),
      renderer(rc.getRenderer()),
      world("back", Gamedata::getInstance().getXmlInt("back/factor")),
      cloud1("cloud1", Gamedata::getInstance().getXmlInt("cloud1/factor")),
      cloud2("cloud2", Gamedata::getInstance().getXmlInt("cloud2/factor")),
      viewport(Viewport::getInstance()),
      currentSprite(0),
      makeVideo(false) {
  const auto pacman = new TwoWayMultiSprite("Pacman");
  pacman->setScale(2);
  sprites.emplace_back(new Sprite("YellowStar"));
  sprites.emplace_back(new MultiSprite("SpinningStar"));
  sprites.emplace_back(pacman);
  Viewport::getInstance().setObjectToTrack(pacman);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  world.draw();
  cloud1.draw();
  cloud2.draw();

  for (const auto &sprite : sprites) sprite->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for (const auto &sprite : sprites) sprite->update(ticks);

  world.update();
  cloud1.update();
  cloud2.update();
  viewport.update();  // always update viewport last
}

void Engine::switchSprite() {
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8 *keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while (!done) {
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
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        } else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if (ticks > 0) {
      clock.incrFrame();
      draw();
      update(ticks);
      if (makeVideo) {
        frameGen.makeFrame();
      }
    }
  }
}
