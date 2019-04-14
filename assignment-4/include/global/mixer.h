#pragma once
#include "util/audio.h"

class Global;

class Mixer {
 public:
  Mixer(const Mixer &) = delete;
  Mixer &operator=(const Mixer &) = delete;

  const Music background;

 private:
  friend class Global;
  Mixer(Gamedata &gamedata);
};