#include "sprite/multisprite.h"
#include "global/gameData.h"
#include "global/imageFactory.h"

MultiSprite::MultiSprite(const std::string& name)
    : images(ImageFactory::getInstance().getImages(name)),
      currentFrame(0),
      numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/frames")),
      interval(Gamedata::getInstance().getXmlInt(name + "/interval")) {}

void MultiSprite::draw() const { images[currentFrame]->draw(getPosition()); }

void MultiSprite::update(Uint32 ticks) {
  if (interval == 0) return;

  elapsed += ticks;
  if (elapsed > interval) {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    elapsed -= interval;
  }
}
