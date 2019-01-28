#include "basicShapes/circle.h"
#include <math.h>

Circle::Circle(int cx, int cy, int r, const std::vector<Shape *> &children)
    : Shape(cx - r, cy - r, 2 * r, 2 * r, children), cx(cx), cy(cy), r(r) {}

inline double degToRad(double x) { return x * M_PI / 180; }

void Circle::draw(const Renderer &renderer) const {
  renderer.setDrawColor(255, 0, 0, 255);
  for (int theta = 0; theta < 360; theta++) {
    renderer.drawPoint(cx + r * cos(degToRad(theta)),
                       cy + r * sin(degToRad(theta)));
  }
}