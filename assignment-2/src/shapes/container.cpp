#include "shapes/container.h"

/** Builder's codes */
ContainerBuilder &ContainerBuilder::add(std::shared_ptr<const Shape> child) {
  this->children.push_back(child);
  return *this;
}

std::shared_ptr<const Container> ContainerBuilder::build() {
  return std::make_shared<const Container>(*this);
}

Container::Container(const ContainerBuilder &b)
    : Shape(b.x, b.y, b.w, b.h, b.children) {}

void Container::draw(const Renderer &renderer) const {
  // do nothing
  (void)renderer;
}