#pragma once
#include <vector>
#include "core/interface.h"
#include "die.h"
#include "player.h"
#include "util/vector2f.h"

class GameScreen;
class NetGameScreen;

// 5 dice, set position into a line with starting point and degree

class Dice : public Drawable {
 public:
  Dice(const Vector2f& position, double theta, unsigned int numDice = 5,
       int g = 50);

  // Draw dice to screen
  void draw() const;

  // Get the dice
  const std::vector<Die>& getDice() const;

  // Set all dice to hide face
  void hide();

 protected:
  // Set all dice to show face
  void show();

  // Randomize the dice
  void roll();

  // Remove a die from dice
  void remove();

  // Set Dice Faces
  void set(const std::vector<int>&);

 private:
  friend class GameScreen;
  friend class NetGameScreen;
  std::vector<Die> dice;
  int gap;

  // have dice positioned in line starting at given position with given angle
  void positionDie(const Vector2f&, double);
};