#pragma once
#include "util/audio.h"

class Global;

class Mixer {
 public:
  Mixer(const Mixer &) = delete;
  Mixer &operator=(const Mixer &) = delete;

  const Music background;
  const Chunk keystroke;
  const Chunk transition;
  const Chunk enterBet;
  const Chunk finish;

 private:
  friend class Global;
  Mixer(Gamedata &gamedata);
};