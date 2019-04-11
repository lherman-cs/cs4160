#include "sprite/sprite.h"
#include <cmath>
#include <functional>
#include <random>
#include "global/gameData.h"
#include "global/imageFactory.h"

Sprite::Sprite(const std::string& name)
    : image(ImageFactory::getInstance().getImage(name)) {}

void Sprite::draw() const { image->draw(getPosition()); }
void Sprite::draw(const Vector2f& pos) const { image->draw(pos); }
