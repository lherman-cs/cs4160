#include "global/clock.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include "global/gameData.h"

Clock::Clock(bool frameCapOn, int period)
    : started(false),
      paused(false),
      FRAME_CAP_ON(frameCapOn),
      PERIOD(period),
      frames(0),
      timeAtStart(0),
      timeAtPause(0),
      currTicks(0),
      prevTicks(0),
      ticks(0) {
  std::cout << "[clock] finished initializing" << std::endl;
  startClock();
}

void Clock::toggleSloMo() {
  throw(std::string("Slow motion is not implemented yet"));
}

unsigned int Clock::getTicks() const {
  if (paused)
    return timeAtPause;
  else
    return SDL_GetTicks() - timeAtStart;
}

unsigned int Clock::getElapsedTicks() {
  if (paused) return 0;

  currTicks = getTicks();
  ticks = currTicks - prevTicks;

  if (FRAME_CAP_ON) {
    // DO DELAY SO NOT TO OCCUPY CPU
    if (ticks < PERIOD) return 0;
    prevTicks = currTicks;
    return ticks;
  } else {
    prevTicks = currTicks;
    return ticks;
  }
}

int Clock::getFps() const {
  if (getSeconds() > 0)
    return frames / getSeconds();
  else
    return 0;
}

void Clock::incrFrame() {
  if (!paused) {
    ++frames;
  }
}

void Clock::startClock() {
  started = true;
  paused = false;
  frames = 0;
  timeAtPause = timeAtStart = SDL_GetTicks();
  prevTicks = 0;
}
void Clock::pause() {
  if (started && !paused) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    paused = true;
  }
}
void Clock::unpause() {
  if (started && paused) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    paused = false;
  }
}