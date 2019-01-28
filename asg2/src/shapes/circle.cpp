#include "shapes/circle.h"
#include <math.h>
#include "renderer/color.h"

Circle::Circle(const CircleBuilder &b)
    : Shape(b.cx - b.r, b.cy - b.r, 2 * b.r, 2 * b.r, b.children), b(b) {}

inline double degToRad(double x) { return x * M_PI / 180; }

void Circle::draw(const Renderer &renderer) const {
  switch (b.fillMode) {
    case Fill::SOLID:
      renderer.setDrawColor(b.fillColor, 255);
      for (int w = 0; w < b.r * 2; w++) {
        for (int h = 0; h < b.r * 2; h++) {
          int dx = b.r - w;  // horizontal offset
          int dy = b.r - h;  // vertical offset
          if ((dx * dx + dy * dy) <= (b.r * b.r)) {
            renderer.drawPoint(b.cx + dx, b.cy + dy);
          }
        }
      }
    case Fill::NONE:
      renderer.setDrawColor(b.borderColor, 255);
      for (int theta = 0; theta < 360; theta++) {
        renderer.drawPoint(b.cx + b.r * cos(degToRad(theta)),
                           b.cy + b.r * sin(degToRad(theta)));
      }
      break;
  }
}

/** Builder's codes */
CircleBuilder &CircleBuilder::add(std::shared_ptr<const Shape> child) {
  this->children.push_back(child);
  return *this;
}

CircleBuilder &CircleBuilder::setBorderColor(const Color &c) {
  this->borderColor = c;
  return *this;
}

CircleBuilder &CircleBuilder::setFillColor(const Color &c) {
  this->fillColor = c;
  return *this;
}

CircleBuilder &CircleBuilder::setFillMode(const Fill &fillMode) {
  this->fillMode = fillMode;
  return *this;
}

std::shared_ptr<const Circle> CircleBuilder::build() {
  return std::make_shared<const Circle>(*this);
}