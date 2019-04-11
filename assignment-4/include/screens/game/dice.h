#pragma once
#include <vector>
#include "core/interface.h"
#include "die.h"
#include "player.h"
#include "util/vector2f.h"

// 5 dice, set position into a line with starting point and degree

class Dice : public Drawable {
 public:
  Dice(const Vector2f& position, double theta, long unsigned int numDice = 5,
       int g = 50);

  // Draw dice to screen
  void draw() const;

  // Set all dice to show face
  void show();

  // Set all dice to hide face
  void hide();

  // Randomize the dice
  void roll();

  // Remove a die from dice
  void remove();

  // Get the dice
  const std::vector<Die>& getDice() const;

 private:
  std::vector<Die> dice;
  int gap;

  // have dice positioned in line starting at given position with given angle
  void positionDie(const Vector2f&, double);
};