#pragma once
#include "sprite/multisprite.h"
#include "sprite/sprite.h"
#include "util/ioMod.h"
#include "widget.h"

class Loading : public Widget {
 public:
  Loading(const Loading&) = delete;
  Loading& operator=(const Loading&) = delete;
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  Loading& create(std::string text);
  static Loading& getInstance();

 private:
  Loading();
  std::string text{"Loading..."};
  Sprite modal{"widget/modal"};
  MultiSprite loading{"widget/loading"};

  IoMod writer{45};
  SDL_Color loadingColor{182, 148, 103, 255};
};