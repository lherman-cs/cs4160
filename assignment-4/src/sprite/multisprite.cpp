#include "sprite/multisprite.h"
#include "global/gameData.h"
#include "global/imageFactory.h"

MultiSprite::MultiSprite(const std::string& name)
    : Drawable(Vector2f(0, 0)),
      images(ImageFactory::getInstance().getImages(name)),
      currentFrame(0),
      numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/frames")) {}

void MultiSprite::draw() const { images[currentFrame]->draw(getPosition()); }

void MultiSprite::update(Uint32 ticks) { (void)(ticks); }
