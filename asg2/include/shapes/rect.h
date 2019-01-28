#pragma once

#include "renderer/color.h"
#include "renderer/shape.h"

class Rect;

class RectBuilder {
 public:
  RectBuilder(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
  RectBuilder &add(const Shape *child);
  RectBuilder &setBorderColor(const Color &c);
  RectBuilder &setFillColor(const Color &c);
  RectBuilder &setFillMode(const Fill &fillMode);
  Rect *build();

 private:
  const int x, y, w, h;
  std::vector<const Shape *> children;
  Color borderColor = Colors::BLACK;
  Color fillColor = Colors::GOPHER;
  Fill fillMode = Fill::SOLID;

  friend Rect;
};

class Rect : public Shape {
 public:
  Rect(const RectBuilder &b);
  void draw(const Renderer &renderer) const;

 private:
  const RectBuilder b;
};