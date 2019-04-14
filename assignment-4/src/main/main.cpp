#include <ctime>
#include "core/engine.h"
#include "core/event.h"
#include "global/clock.h"
#include "global/gameData.h"
#include "global/imageFactory.h"
#include "global/mixer.h"
#include "global/navigator.h"
#include "global/renderContext.h"
#include "global/viewport.h"

static void init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw(std::string("Could not init SDL: ") + SDL_GetError());
  }

  RenderContext::getInstance();
  Mixer::getInstance();
  Clock::getInstance();
  ImageFactory::getInstance();
  Navigator::getInstance();
  Viewport::getInstance();
}

int main(int, char*[]) {
  srand(time(NULL));

  // init();

  try {
    Engine engine;
    engine.play();
  } catch (const string& msg) {
    std::cout << msg << std::endl;
  } catch (...) {
    std::cout << "Oops, someone threw an exception!" << std::endl;
  }

  // SDL_Quit();
  return 0;
}
