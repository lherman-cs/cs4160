#pragma once

#include "renderer/color.h"
#include "renderer/shape.h"

class Line;

class LineBuilder {
 public:
  LineBuilder(int x1, int y1, int x2, int y2)
      : x1(x1), y1(y1), x2(x2), y2(y2) {}
  LineBuilder &add(std::shared_ptr<const Shape> child);
  LineBuilder &setFillColor(const Color &c);
  std::shared_ptr<const Line> build();

 private:
  const int x1, y1, x2, y2;
  std::vector<std::shared_ptr<const Shape>> children;
  Color fillColor = Colors::BLACK;

  friend Line;
};

class Line : public Shape {
 public:
  Line(const LineBuilder &b);
  void draw(const Renderer &renderer) const;

 private:
  const LineBuilder b;
};