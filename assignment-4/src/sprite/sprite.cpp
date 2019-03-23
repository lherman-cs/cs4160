#include "sprite/sprite.h"
#include <cmath>
#include <functional>
#include <random>
#include "global/gameData.h"
#include "global/imageFactory.h"

Sprite::Sprite(const std::string& name)
    : Drawable(
          name,
          Vector2f(
              Gamedata::getInstance().getXmlInt(name + "/start-location/x"),
              Gamedata::getInstance().getXmlInt(name + "/start-location/y")),
          Vector2f(Gamedata::getInstance().getXmlInt(name + "/speed/x"),
                   Gamedata::getInstance().getXmlInt(name + "/speed/y"))),
      image(ImageFactory::getInstance().getImage(name)) {}

void Sprite::draw() const { image->draw(getPosition()); }

void Sprite::update(Uint32 ticks) {}
