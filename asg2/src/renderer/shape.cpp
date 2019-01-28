#include "renderer/shape.h"

Shape::Shape(int x, int y, int w, int h, const std::vector<Shape *> &children)
    : x(x), y(y), w(w), h(h), children(children) {}

Shape::~Shape() {
  for (auto child : children) delete child;
}