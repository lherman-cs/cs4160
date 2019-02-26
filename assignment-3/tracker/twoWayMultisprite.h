#pragma once
#include "multisprite.h"

class TwoWayMultiSprite : public MultiSprite {
 public:
  TwoWayMultiSprite(const std::string& name);
  TwoWayMultiSprite(const TwoWayMultiSprite&) = delete;
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&) = delete;
  virtual void update(Uint32 ticks);

 protected:
  std::vector<Image*> leftSprites;
  std::vector<Image*> rightSprites;
  bool facingRight;
};