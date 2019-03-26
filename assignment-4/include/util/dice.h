#include <vector>
#include "core/interface.h"
#include "player/player.h"
#include "sprite/die.h"
#include "util/vector2f.h"

// 5 dice, set position into a line with starting point and degree

class Dice : public Drawable {
 public:
  Dice(long unsigned int numDice = 5, int g = 50);

  // Draw dice to screen
  void draw() const;

  // Set all dice to show face
  void show();

  // have dice positioned in line starting at given position with given angle
  void positionDie(const Vector2f&, double);
  void roll();

 private:
  std::vector<Die> dice;
  int gap;
};