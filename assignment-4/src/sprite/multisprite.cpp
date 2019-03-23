#include "sprite/multisprite.h"
#include "global/gameData.h"
#include "global/imageFactory.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

MultiSprite::MultiSprite(const std::string& name)
    : Drawable(
          name,
          Vector2f(
              Gamedata::getInstance().getXmlInt(name + "/start-location/x"),
              Gamedata::getInstance().getXmlInt(name + "/start-location/y")),
          Vector2f(Gamedata::getInstance().getXmlInt(name + "/speed/x"),
                   Gamedata::getInstance().getXmlInt(name + "/speed/y"))),
      images(ImageFactory::getInstance().getImages(name)),
      currentFrame(0),
      numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/frames")),
      frameInterval(
          Gamedata::getInstance().getXmlInt(name + "/frame-interval")),
      timeSinceLastFrame(0) {}

void MultiSprite::draw() const { images[currentFrame]->draw(getPosition()); }

void MultiSprite::update(Uint32 ticks) { advanceFrame(ticks); }
