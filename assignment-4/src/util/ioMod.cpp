#include "util/ioMod.h"
#include <SDL2/SDL_image.h>
#include "global/gameData.h"
#include "global/renderContext.h"

IoMod::~IoMod() {
  TTF_CloseFont(font);
  TTF_Quit();
}

IoMod::IoMod(int size)
    : init(TTF_Init()),
      renderer(RenderContext::getInstance().getRenderer()),
      font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
                        size)) {
  if (init == -1) {
    throw std::string("error: Couldn't init font");
  }
  if (font == NULL) {
    throw std::string("error: font not found");
  }
}

void IoMod::writeText(const std::string& msg, int x, int y,
                      SDL_Color textColor) const {
  SDL_Surface* surface = TTF_RenderText_Solid(font, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}
