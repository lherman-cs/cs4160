#include "shapes/gopher.h"

GopherBuilder &GopherBuilder::add(const Shape *child) {
  this->children.push_back(child);
  return *this;
}

GopherBuilder &GopherBuilder::setBorderColor(const Color &c) {
  this->borderColor = c;
  return *this;
}

GopherBuilder &GopherBuilder::setFillColor(const Color &c) {
  this->fillColor = c;
  return *this;
}

GopherBuilder &GopherBuilder::setFillMode(const Fill &fillMode) {
  this->fillMode = fillMode;
  return *this;
}

Gopher *GopherBuilder::build() { return new Gopher(*this); }

/** Gopher shape implementation */
Gopher::Gopher(const GopherBuilder &b)
    : Shape(b.x, b.y, b.w, b.h, b.children), b(b) {}

void Gopher::draw(const Renderer &renderer) const {}