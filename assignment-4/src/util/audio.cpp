#include "util/audio.h"
#include "global/gameData.h"

Music::Music(const std::string name)
    : music(Mix_LoadMUS(Gamedata::getInstance().getXmlStr(name).c_str())) {
  if (music == NULL) throw("[audio/music] failed to initialize " + name);
}

Music::~Music() { Mix_FreeMusic(music); }

void Music::play() const {
  if (Mix_PlayMusic(music, -1) == -1) {
    throw("[audio/music] failed to play music");
  }
}

Chunk::Chunk(const std::string name)
    : chunk(Mix_LoadWAV(Gamedata::getInstance().getXmlStr(name).c_str())) {
  if (chunk == NULL) throw("[audio/chunk] failed to initialize " + name);
}

Chunk::~Chunk() { Mix_FreeChunk(chunk); }