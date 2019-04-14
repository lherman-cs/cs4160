#include "global/global.h"

Global& Global::get() {
  static Global global;
  return global;
}

static bool init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw(std::string("Could not init SDL: ") + SDL_GetError());
  }

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
                    MIX_DEFAULT_CHANNELS, 4096) == -1) {
    throw("[mixer] failed to initialize");
  }

  return true;
}

Global::Global()
    : initialized(init()),
      gamedata(),
      clock(gamedata.getXmlBool("frameCapOn"), gamedata.getXmlInt("period")),
      renderContext(gamedata),
      imageFactory(gamedata),
      viewport(gamedata),
      widget(),
      mixer(gamedata),
      navigator(),
      promise() {
  std::cout << "Global is initialized" << std::endl;
}

Global::~Global() {
  Mix_CloseAudio();
  SDL_Quit();
}