#pragma once
#include "util/audio.h"

class Mixer {
 public:
  static Mixer &getInstance();
  Mixer(const Mixer &) = delete;
  Mixer &operator=(const Mixer &) = delete;

  const Music background;

 private:
  Mixer();
};