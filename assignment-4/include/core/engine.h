#include <SDL2/SDL.h>
#include <memory>
#include <unordered_map>
#include "global/clock.h"
#include "global/navigator.h"
#include "global/renderContext.h"
#include "global/viewport.h"
#include "screens/screen.h"
#include "util/world.h"

class Engine {
 public:
  Engine();
  ~Engine();
  void forward(bool& done);
  void play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

 private:
  const RenderContext& rc;
  Clock& clock;
  Navigator& navigator;

  SDL_Renderer* const renderer;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
