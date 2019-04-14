#include "sprite/sprite.h"
#include <cmath>
#include <functional>
#include <random>
#include "global/global.h"

Sprite::Sprite(const std::string& name)
    : image(Global::get().imageFactory.getImage(name)) {}

void Sprite::draw() const { image->draw(getPosition()); }
void Sprite::draw(const Vector2f& pos) const { image->draw(pos); }
