#include "shapes/circle.h"
#include <math.h>
#include "renderer/color.h"

Circle::Circle(const CircleBuilder &b)
    : Shape(b.x, b.y, 2 * b.r, 2 * b.r, b.children), b(b) {}

inline double degToRad(double x) { return x * M_PI / 180; }

void Circle::draw(const Renderer &renderer) const {
  auto cx = b.x + w / 2;
  auto cy = b.y + h / 2;

  auto outline = [&] {
    renderer.setDrawColor(b.borderColor, 255);
    for (int theta = 0; theta < 360; theta++) {
      renderer.drawPoint(cx + b.r * cos(degToRad(theta)),
                         cy + b.r * sin(degToRad(theta)));
    }
  };

  auto fill = [&] {
    renderer.setDrawColor(b.fillColor, 255);
    for (int w = 0; w < b.r * 2; w++) {
      for (int h = 0; h < b.r * 2; h++) {
        int dx = b.r - w;  // horizontal offset
        int dy = b.r - h;  // vertical offset
        if ((dx * dx + dy * dy) <= (b.r * b.r)) {
          renderer.drawPoint(cx + dx, cy + dy);
        }
      }
    }
  };

  switch (b.fillMode) {
    case Fill::NONE:
      outline();
      break;
    case Fill::SOLID:
      fill();
      break;
    case Fill::SOLID_OUTLINED:
      fill();
      outline();
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