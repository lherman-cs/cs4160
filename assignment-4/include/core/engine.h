#include <SDL2/SDL.h>
#include <vector>
#include "global/clock.h"
#include "global/ioMod.h"
#include "global/renderContext.h"
#include "global/viewport.h"
#include "util/world.h"

class Engine {
 public:
  Engine();
  ~Engine();
  void play();
  void switchSprite();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

 private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer* const renderer;
  World background;
  World waitress;
  World hangingLightBulb;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  int currentSprite;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
