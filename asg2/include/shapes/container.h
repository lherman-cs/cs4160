#pragma once

#include "renderer/shape.h"

class Container : public Shape {
 public:
  Container(int x, int y, int w, int h,
            const std::vector<std::shared_ptr<const Shape>> &children);
  void draw(const Renderer &renderer) const;

 private:
};