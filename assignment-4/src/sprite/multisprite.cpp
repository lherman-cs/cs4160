#include "sprite/multisprite.h"
#include "global/global.h"

MultiSprite::MultiSprite(const std::string& name)
    : images(Global::get().imageFactory.getImages(name)),
      currentFrame(0),
      numberOfFrames(Global::get().gamedata.getXmlInt(name + "/frames")),
      interval(Global::get().gamedata.getXmlInt(name + "/interval")),
      loop(Global::get().gamedata.getXmlInt(name + "/loop")) {}

void MultiSprite::draw() const {
  if (loop == 0) return;
  images[currentFrame]->draw(getPosition());
}

void MultiSprite::update(Uint32 ticks) {
  if (interval == 0) return;
  if (loop == 0) return;

  elapsed += ticks;
  if (elapsed > interval) {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    if (currentFrame == 0 && loop > 0) loop--;
    elapsed -= interval;
  }
}
