#pragma once

#include "renderer/color.h"
#include "renderer/shape.h"

class Circle;

class CircleBuilder {
 public:
  CircleBuilder(int cx, int cy, int r) : cx(cx), cy(cy), r(r) {}
  CircleBuilder &add(std::shared_ptr<const Shape> child);
  CircleBuilder &setBorderColor(const Color &c);
  CircleBuilder &setFillColor(const Color &c);
  CircleBuilder &setFillMode(const Fill &fillMode);
  std::shared_ptr<const Circle> build();

 private:
  const int cx, cy, r;
  std::vector<std::shared_ptr<const Shape>> children;
  Color borderColor = Colors::BLACK;
  Color fillColor = Colors::WHITE;
  Fill fillMode = Fill::NONE;

  friend Circle;
};

class Circle : public Shape {
 public:
  Circle(const CircleBuilder &b);
  void draw(const Renderer &renderer) const;

 private:
  const CircleBuilder b;
};