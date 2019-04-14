#include "sprite/multisprite.h"
#include "global/global.h"

MultiSprite::MultiSprite(const std::string& name)
    : images(Global::get().imageFactory.getImages(name)),
      currentFrame(0),
      numberOfFrames(Global::get().gamedata.getXmlInt(name + "/frames")),
      interval(Global::get().gamedata.getXmlInt(name + "/interval")) {}

void MultiSprite::draw() const { images[currentFrame]->draw(getPosition()); }

void MultiSprite::update(Uint32 ticks) {
  if (interval == 0) return;

  elapsed += ticks;
  if (elapsed > interval) {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    elapsed -= interval;
  }
}
