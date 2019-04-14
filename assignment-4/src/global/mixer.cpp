#include "global/mixer.h"

Mixer::Mixer(Gamedata& gamedata)
    : background(Music(gamedata.getXmlStr("mixer/music/background"))) {
  std::cout << "[mixer] finished initializing" << std::endl;
}