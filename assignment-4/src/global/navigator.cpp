#include "global/navigator.h"
#include <iostream>

Navigator& Navigator::getInstance() {
  static Navigator navigator;
  return navigator;
}

Navigator::Navigator() : currentScreen(), history() {}

void Navigator::pop() {
  if (history.size() == 0) return;
  history.pop();
  currentScreen = history.top();
}

std::shared_ptr<Screen> Navigator::getCurrentScreen() { return currentScreen; }