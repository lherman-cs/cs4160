#include "global/imageFactory.h"
#include "sprite.h"
#include "util/vector2f.h"

class Die {
 public:
  enum State { visible, invisible, hidden };

  Die();
  Die(const Vector2f&, State, int val = 0);
  Die(const Die&) = delete;
  Die& operator=(const Die&) = delete;
  void draw();
  void update(Uint32 ticks);
  Die& set(const Vector2f& position);
  Die& set(const Die::State state);
  Die& set(int value);

  static int getSize();

 private:
  std::vector<Image*> faces{ImageFactory::getInstance().getImages("dice")};
  Sprite hide{"hidden-dice"};
  Sprite gone{"invisible-dice"};
  State state = visible;
  int value = 0;
  Vector2f position = Vector2f(0, 0);
};