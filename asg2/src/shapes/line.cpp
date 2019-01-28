#include "shapes/line.h"

#include <math.h>

LineBuilder &LineBuilder::add(std::shared_ptr<const Shape> child) {
  this->children.push_back(child);
  return *this;
}

LineBuilder &LineBuilder::setFillColor(const Color &c) {
  this->fillColor = c;
  return *this;
}

std::shared_ptr<const Line> LineBuilder::build() {
  return std::make_shared<const Line>(*this);
}

/** Line shape implementation */
Line::Line(const LineBuilder &b)
    : Shape(std::min(b.x1, b.x2), std::min(b.y1, b.y2), std::abs(b.x2 - b.x1),
            std::abs(b.y2 - b.y1), b.children),
      b(b) {}

void Line::draw(const Renderer &renderer) const {
  renderer.setDrawColor(b.fillColor, 255);
  renderer.drawLine(b.x1, b.y1, b.x2, b.y2);
}