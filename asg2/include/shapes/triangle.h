#pragma once

#include "renderer/color.h"
#include "renderer/shape.h"

class Triangle;

class TriangleBuilder {
 public:
  TriangleBuilder(int x1, int y1, int x2, int y2, int x3, int y3)
      : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {}
  TriangleBuilder &add(std::shared_ptr<const Shape> child);
  TriangleBuilder &setBorderColor(const Color &c);
  TriangleBuilder &setFillColor(const Color &c);
  TriangleBuilder &setFillMode(const Fill &fillMode);
  std::shared_ptr<const Triangle> build();

 private:
  const int x1, y1, x2, y2, x3, y3;
  std::vector<std::shared_ptr<const Shape>> children;
  Color borderColor = Colors::BLACK;
  Color fillColor = Colors::GOPHER;
  Fill fillMode = Fill::SOLID;

  friend Triangle;
};

class Triangle : public Shape {
 public:
  Triangle(const TriangleBuilder &b);
  void draw(const Renderer &renderer) const;

 private:
  const TriangleBuilder b;
};