#include "global/navigator.h"
#include "screens/intro.h"

Navigator& Navigator::getInstance() {
  static Navigator navigator;
  return navigator;
}

Navigator::Navigator() : currentScreen(), history() { push<IntroScreen>(); }

void Navigator::pop() {
  if (history.size() == 0) return;
  currentScreen = history.top();
  history.pop();
}

std::shared_ptr<Screen> Navigator::getCurrentScreen() { return currentScreen; }