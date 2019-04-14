#include "global/mixer.h"

static void init() {
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
                    MIX_DEFAULT_CHANNELS, 4096) == -1) {
    throw("[mixer] failed to initialize");
  }
}

Mixer::Mixer()
    : background((init(),  // this is just a hack so that SDL_Mixer will be
                           // initialized first. No need to repeat this.
                  Music("mixer/music/background"))) {}

Mixer& Mixer::getInstance() {
  static Mixer instance;
  return instance;
}