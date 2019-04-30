#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

class Mixer;

/* ignore effc++ since faces is just going to be used */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class Music {
 public:
  ~Music();
  void play() const;
  void pause() const;
  void resume() const;

 private:
  friend class Mixer;
  Music(const std::string name);
  Mix_Music* music;
};

class Chunk {
 public:
  ~Chunk();
  void play() const;

 private:
  friend class Mixer;
  Chunk(const std::string name);
  Mix_Chunk* chunk;
};

#pragma GCC diagnostic pop