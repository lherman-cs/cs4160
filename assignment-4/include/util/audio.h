#pragma once
#ifndef __EMSCRIPTEN__
#include <SDL2/SDL_mixer.h>
#endif
#include <string>

#ifdef __EMSCRIPTEN__
using Mix_Music = int;  // fake type
using Mix_Chunk = int;  // fake type
#define MIX_DEFAULT_FREQUENCY 0
#define MIX_DEFAULT_FORMAT 0
#define MIX_DEFAULT_CHANNELS 0
inline Mix_Music* Mix_LoadMUS(const char* _1) { return new int; }
inline void Mix_FreeMusic(Mix_Music* _1) { delete _1; }
inline Mix_Chunk* Mix_LoadWAV(const char* _1) { return new int; }
inline void Mix_FreeChunk(Mix_Chunk* _1) { delete _1; }
inline int Mix_PlayMusic(Mix_Music* _1, int _2) { return 0; }
inline int Mix_OpenAudio(int _1, unsigned _2, int _3, int _4) { return 0; }
inline void Mix_CloseAudio() {}
#endif

class Mixer;

/* ignore effc++ since faces is just going to be used */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class Music {
 public:
  ~Music();
  void play() const;

 private:
  friend class Mixer;
  Music(const std::string name);
  Mix_Music* music;
};

class Chunk {
 public:
  ~Chunk();

 private:
  friend class Mixer;
  Chunk(const std::string name);
  Mix_Chunk* chunk;
};

#pragma GCC diagnostic pop