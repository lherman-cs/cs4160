#pragma once
#include "widget.h"

class WidgetController;

class Token : public Widget {
 public:
  Token();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void set(const unsigned int& index);

 private:
  friend class WidgetController;
  Sprite token{"widget/token"};

  std::vector<Vector2f> positions{Vector2f{490, 590}, Vector2f{655, 210},
                                  Vector2f{336, 213}, Vector2f{297, 438},
                                  Vector2f{704, 432}};
};