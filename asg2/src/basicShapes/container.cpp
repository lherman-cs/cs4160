#include "basicShapes/container.h"

Container::Container(int x, int y, int w, int h,
                     const std::vector<Shape *> &children)
    : Shape(x, y, w, h, children) {}

void Container::draw(const Renderer &renderer) const {}