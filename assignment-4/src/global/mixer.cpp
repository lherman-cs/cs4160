#include "global/mixer.h"

Mixer::Mixer(Gamedata& gamedata)
    : background(Music(gamedata.getXmlStr("mixer/music/background"))),
      keystroke(Chunk(gamedata.getXmlStr("mixer/chunk/keystroke"))),
      transition(Chunk(gamedata.getXmlStr("mixer/chunk/transition"))),
      enterBet(Chunk(gamedata.getXmlStr("mixer/chunk/enter-bet"))) {
  std::cout << "[mixer] finished initializing" << std::endl;
}