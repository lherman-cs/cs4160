#include "shapes/rect.h"

RectBuilder &RectBuilder::add(std::shared_ptr<const Shape> child) {
  this->children.push_back(child);
  return *this;
}

RectBuilder &RectBuilder::setBorderColor(const Color &c) {
  this->borderColor = c;
  return *this;
}

RectBuilder &RectBuilder::setFillColor(const Color &c) {
  this->fillColor = c;
  return *this;
}

RectBuilder &RectBuilder::setFillMode(const Fill &fillMode) {
  this->fillMode = fillMode;
  return *this;
}

std::shared_ptr<const Rect> RectBuilder::build() {
  return std::make_shared<const Rect>(*this);
}

/** Rect shape implementation */
Rect::Rect(const RectBuilder &b)
    : Shape(b.x, b.y, b.w, b.h, b.children), b(b) {}

void Rect::draw(const Renderer &renderer) const {
  switch (b.fillMode) {
    case Fill::SOLID_OUTLINED:
      renderer.setDrawColor(b.fillColor, 255);
      renderer.fillRect(b.x, b.y, b.w, b.h);
      renderer.setDrawColor(b.borderColor, 255);
      renderer.drawRect(b.x, b.y, b.w, b.h);
      break;
    case Fill::SOLID:
      renderer.setDrawColor(b.fillColor, 255);
      renderer.fillRect(b.x, b.y, b.w, b.h);
      break;
    case Fill::NONE:
      renderer.setDrawColor(b.borderColor, 255);
      renderer.drawRect(b.x, b.y, b.w, b.h);
      break;
  }
}