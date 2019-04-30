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

  std::vector<Vector2f> positions{Vector2f{505, 570}, Vector2f{316, 428},
                                  Vector2f{351, 232}, Vector2f{647, 235},
                                  Vector2f{682, 432}};
};