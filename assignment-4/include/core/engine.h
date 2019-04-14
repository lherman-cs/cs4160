#include <SDL2/SDL.h>
#include <memory>
#include <unordered_map>
#include "global/global.h"
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
  const Mixer& mixer;
  Clock& clock;
  Navigator& navigator;
  PromiseScheduler& promiseScheduler;
  WidgetController& widget;

  SDL_Renderer* const renderer;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
