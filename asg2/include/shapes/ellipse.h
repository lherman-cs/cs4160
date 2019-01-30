#pragma once

#include "renderer/color.h"
#include "renderer/shape.h"

class Ellipse;

class EllipseBuilder {
 public:
  EllipseBuilder(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
  EllipseBuilder &add(std::shared_ptr<const Shape> child);
  EllipseBuilder &setBorderColor(const Color &c);
  EllipseBuilder &setFillColor(const Color &c);
  EllipseBuilder &setFillMode(const Fill &fillMode);
  std::shared_ptr<const Ellipse> build();

 private:
  const int x, y, w, h;
  std::vector<std::shared_ptr<const Shape>> children;
  Color borderColor = Colors::BLACK;
  Color fillColor = Colors::WHITE;
  Fill fillMode = Fill::NONE;

  friend Ellipse;
};

class Ellipse : public Shape {
 public:
  Ellipse(const EllipseBuilder &b);
  void draw(const Renderer &renderer) const;

 private:
  const EllipseBuilder b;
};