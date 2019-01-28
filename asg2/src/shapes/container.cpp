#include "shapes/container.h"

Container::Container(int x, int y, int w, int h,
                     const std::vector<const Shape *> &children)
    : Shape(x, y, w, h, children) {}

void Container::draw(const Renderer &renderer) const {}