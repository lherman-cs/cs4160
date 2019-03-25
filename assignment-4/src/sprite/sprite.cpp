#include "sprite/sprite.h"
#include <cmath>
#include <functional>
#include <random>
#include "global/gameData.h"
#include "global/imageFactory.h"

Sprite::Sprite(const std::string& name)
    : Drawable(Vector2f(0, 0)),
      image(ImageFactory::getInstance().getImage(name)) {}

void Sprite::draw() const { image->draw(getPosition()); }

void Sprite::update(Uint32 ticks) { (void)ticks; }
