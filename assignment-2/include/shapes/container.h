#pragma once

#include "renderer/shape.h"

class Container;

class ContainerBuilder {
 public:
  ContainerBuilder(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
  ContainerBuilder &add(std::shared_ptr<const Shape> child);
  std::shared_ptr<const Container> build();

 private:
  const int x, y, w, h;
  std::vector<std::shared_ptr<const Shape>> children;

  friend Container;
};

class Container : public Shape {
 public:
  Container(const ContainerBuilder &b);
  void draw(const Renderer &renderer) const;

 private:
};