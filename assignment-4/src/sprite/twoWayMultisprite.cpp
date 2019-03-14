#include "sprite/twoWayMultisprite.h"
#include "global/imageFactory.h"
#include "sprite/multisprite.h"

TwoWayMultiSprite::TwoWayMultiSprite(const std::string& name)
    : MultiSprite(name, "/right"),
      leftSprites(ImageFactory::getInstance().getImages(name + "/left")),
      rightSprites(images),
      facingRight(true) {}

void TwoWayMultiSprite::update(Uint32 ticks) {
  MultiSprite::update(ticks);

  if (getVelocityX() < 0) {
    if (facingRight) {
      images = leftSprites;
      facingRight = false;
    }
  } else {
    if (!facingRight) {
      images = rightSprites;
      facingRight = true;
    }
  }
}