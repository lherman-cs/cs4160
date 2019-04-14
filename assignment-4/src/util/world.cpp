#include "util/world.h"
#include <iostream>
#include "global/global.h"

World::World(const std::string& name, int fact)
    : image(Global::get().imageFactory.getImage(name)),
      factor(fact),
      imageWidth(image->getWidth()),
      viewX(0.0),
      viewY(0.0),
      view(Global::get().viewport) {}

void World::update() {
  viewX = static_cast<int>(view.getX() / factor) % imageWidth;
  viewY = view.getY();
}

void World::draw() const {
  image->draw(0, 0, -viewX, -viewY);
  image->draw(0, 0, imageWidth - viewX, -viewY);
}
