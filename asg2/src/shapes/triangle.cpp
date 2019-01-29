#include "shapes/triangle.h"

#include <math.h>
#include <algorithm>
#include <iostream>

TriangleBuilder &TriangleBuilder::add(std::shared_ptr<const Shape> child) {
  this->children.push_back(child);
  return *this;
}

TriangleBuilder &TriangleBuilder::setBorderColor(const Color &c) {
  this->borderColor = c;
  return *this;
}

TriangleBuilder &TriangleBuilder::setFillColor(const Color &c) {
  this->fillColor = c;
  return *this;
}

TriangleBuilder &TriangleBuilder::setFillMode(const Fill &fillMode) {
  this->fillMode = fillMode;
  return *this;
}

std::shared_ptr<const Triangle> TriangleBuilder::build() {
  return std::make_shared<const Triangle>(*this);
}

/** Triangle shape implementation */
Triangle::Triangle(const TriangleBuilder &b)
    : Shape(std::min(b.x1, std::min(b.x2, b.x3)),
            std::min(b.y1, std::min(b.y2, b.y3)),
            std::max(std::abs(b.x2 - b.x1), std::abs(b.x3 - b.x1)),
            std::max(std::abs(b.y2 - b.y1), std::abs(b.y3 - b.y1)), b.children),
      b(b) {}

void fillUpper(const Renderer &renderer, const int points[3][2]) {
  // fill up triangles from top to middle

  // toppest = points[0]
  double y1Steps = points[1][1] - points[0][1];
  double y2Steps = points[2][1] - points[0][1];
  if (y1Steps == 0 || y2Steps == 0) return;

  double dx1 = (points[1][0] - points[0][0]) / y1Steps;
  double dx2 = (points[2][0] - points[0][0]) / y2Steps;

  double x1 = points[0][0];
  double x2 = points[0][0];

  for (auto y = points[0][1]; y <= points[1][1]; y++) {
    renderer.drawLine(x1, y, x2, y);
    x1 += dx1;
    x2 += dx2;
  }
}

void fillLower(const Renderer &renderer, const int points[3][2]) {
  // fill up triangles from bottom to middle

  double y1Steps = points[1][1] - points[2][1];
  double y2Steps = points[0][1] - points[2][1];
  if (y1Steps == 0 || y2Steps == 0) return;

  double dx1 = (points[2][0] - points[1][0]) / y1Steps;
  double dx2 = (points[2][0] - points[0][0]) / y2Steps;

  double x1 = points[2][0];
  double x2 = points[2][0];

  for (auto y = points[2][1]; y >= points[1][1]; y--) {
    renderer.drawLine(x1, y, x2, y);
    x1 += dx1;
    x2 += dx2;
  }
}

void Triangle::draw(const Renderer &renderer) const {
  switch (b.fillMode) {
    case Fill::SOLID: {
      renderer.setDrawColor(b.fillColor, 255);
      int points[3][2] = {{b.y1, b.x1}, {b.y2, b.x2}, {b.y3, b.x3}};
      auto ptr = (std::pair<int, int> *)points;
      std::sort(ptr, ptr + 3);
      std::swap(points[0][0], points[0][1]);
      std::swap(points[1][0], points[1][1]);
      std::swap(points[2][0], points[2][1]);

      fillUpper(renderer, points);
      fillLower(renderer, points);
      break;
    }
    case Fill::NONE:
      break;
  }
}