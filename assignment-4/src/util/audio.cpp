#include "util/audio.h"
#include "global/global.h"

Music::Music(const std::string path) : music(Mix_LoadMUS(path.c_str())) {
  if (music == NULL) {
    auto err = "[audio/music] failed to initialize " + path;
    // std::cout << Mix_GetError() << std::endl;
    std::cout << err << std::endl;
    throw(err);
  }
}

Music::~Music() { Mix_FreeMusic(music); }

void Music::play() const {
  if (Mix_PlayMusic(music, -1) == -1) {
    throw("[audio/music] failed to play music");
  }
}

void Music::pause() const { Mix_PauseMusic(); }
void Music::resume() const { Mix_ResumeMusic(); }

Chunk::Chunk(const std::string path) : chunk(Mix_LoadWAV(path.c_str())) {
  if (chunk == NULL) throw("[audio/chunk] failed to initialize " + path);
}

void Chunk::play() const {
  // no need to check an error here. Even if it fails, it's just a sound effect,
  // not a big deal. No need to crash.
  Mix_PlayChannel(-1, chunk, 0);
}

Chunk::~Chunk() { Mix_FreeChunk(chunk); }