#pragma once
#include "widget.h"

class WidgetController;

class Loading : public Widget {
 public:
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  friend class WidgetController;
  Loading(std::string text);
  std::string text{"Loading..."};
  Sprite modal{"widget/modal"};
  MultiSprite loading{"widget/loading"};

  IoMod writer{45};
  SDL_Color loadingColor{182, 148, 103, 255};
};