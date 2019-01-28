#pragma once

#include "renderer/color.h"
#include "renderer/shape.h"

class Gopher;

class GopherBuilder {
 public:
  GopherBuilder(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
  GopherBuilder &add(const Shape *child);
  GopherBuilder &setBorderColor(const Color &c);
  GopherBuilder &setFillColor(const Color &c);
  GopherBuilder &setFillMode(const Fill &fillMode);
  Gopher *build();

 private:
  const int x, y, w, h;
  std::vector<const Shape *> children;
  Color borderColor = Colors::BLACK;
  Color fillColor = Colors::GOPHER;
  Fill fillMode = Fill::SOLID;

  friend Gopher;
};

class Gopher : public Shape {
 public:
  Gopher(const GopherBuilder &b);
  void draw(const Renderer &renderer) const;

 private:
  const GopherBuilder b;
};