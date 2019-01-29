#include "shapes/ellipse.h"
#include <math.h>
#include "renderer/color.h"

Ellipse::Ellipse(const EllipseBuilder &b)
    : Shape(b.cx - b.w / 2, b.cy - b.h / 2, b.w, b.h, b.children), b(b) {}

void Ellipse::draw(const Renderer &renderer) const {
  switch (b.fillMode) {
    case Fill::SOLID: {
      renderer.setDrawColor(b.fillColor, 255);
      int hr = b.h / 2;
      int wr = b.w / 2;
      for (int y = -hr; y <= hr; y++) {
        for (int x = -wr; x <= wr; x++) {
          if (x * x * hr * hr + y * y * wr * wr <= hr * hr * wr * wr)
            renderer.drawPoint(b.cx + x, b.cy + y);
        }
      }
    }

    case Fill::NONE:
      renderer.setDrawColor(b.borderColor, 255);
      int hr = b.h / 2;
      int wr = b.w / 2;
      for (int theta = 0; theta < 360; theta++) {
        renderer.drawPoint(b.cx + wr * cos(theta * M_PI / 180),
                           b.cy + hr * sin(theta * M_PI / 180));
      }
      break;
  }
}

/** Builder's codes */
EllipseBuilder &EllipseBuilder::add(std::shared_ptr<const Shape> child) {
  this->children.push_back(child);
  return *this;
}

EllipseBuilder &EllipseBuilder::setBorderColor(const Color &c) {
  this->borderColor = c;
  return *this;
}

EllipseBuilder &EllipseBuilder::setFillColor(const Color &c) {
  this->fillColor = c;
  return *this;
}

EllipseBuilder &EllipseBuilder::setFillMode(const Fill &fillMode) {
  this->fillMode = fillMode;
  return *this;
}

std::shared_ptr<const Ellipse> EllipseBuilder::build() {
  return std::make_shared<const Ellipse>(*this);
}