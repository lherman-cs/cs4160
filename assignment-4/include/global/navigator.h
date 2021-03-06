#pragma once
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include "screens/screen.h"

class Global;

class Navigator {
 public:
  template <typename T,
            typename = typename std::enable_if<
                std::is_base_of<Screen, T>::value, T>::type,
            class... Args>
  void push(Args&&... args) {
    currentScreen = std::make_shared<T>(args...);
    history.push(currentScreen);
  }
  void pop();
  void reset();
  std::shared_ptr<Screen> getCurrentScreen();

  Navigator(const Navigator&) = delete;
  Navigator& operator=(const Navigator&) = delete;

 private:
  friend class Global;
  Navigator();
  std::shared_ptr<Screen> currentScreen;
  std::stack<std::shared_ptr<Screen>> history;
};