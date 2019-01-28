#include "renderer/shape.h"

Shape::Shape(int x, int y, int w, int h,
             const std::vector<std::shared_ptr<const Shape>> &children)
    : x(x), y(y), w(w), h(h), children(children) {}

Shape::~Shape() {}