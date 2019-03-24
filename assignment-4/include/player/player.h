#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "player/event.h"

class Player {
 protected:
  Player(const std::string &name);

 public:
  virtual ~Player();

  template <typename T>
  void subscribe(EventCallback<T> cb) {
    auto wrapper = [cb](const Event *event) {
      const T *e = static_cast<const T *>(event);
      cb(*e);
    };

    subscribers[typeid(T)].emplace_back(wrapper);
  }

 protected:
  template <typename T>
  void emit(const T event) {
    auto eventSubscribers = subscribers[typeid(T)];
    for (callback cb : eventSubscribers) cb(&event);
  }
  std::string name;

 private:
  using callback = std::function<void(const Event *)>;
  std::unordered_map<std::type_index, std::vector<callback>> subscribers;
};