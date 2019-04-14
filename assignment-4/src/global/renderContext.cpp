#include "global/global.h"

RenderContext::RenderContext(Gamedata& gamedata)
    : gamedata(gamedata), window(initWindow()), renderer(initRenderer()) {}

RenderContext::~RenderContext() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

SDL_Window* RenderContext::initWindow() {
  std::string title = gamedata.getXmlStr("title");
  int width = gamedata.getXmlInt("view/width");
  int height = gamedata.getXmlInt("view/height");
  window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    throw(std::string("Couldn't make a window: ") + SDL_GetError());
  }
  return window;
}

SDL_Renderer* RenderContext::initRenderer() {
  // To test the Clock class's ability to cap the frame rate, use:
  // SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) throw std::string("No Renderer");
  return renderer;
}