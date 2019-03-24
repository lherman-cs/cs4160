#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "core/event.h"

/**
 * Observable allows anybody to subscribe any event. All classes
 * that extend Observable will be able to emit an arbitrary event.
 * Observable can't be created publicly, it has to be extended.
 */
class Observable {
 public:
  template <typename T>
  void subscribe(EventCallback<T> cb) {
    auto wrapper = [cb](const Event *event) {
      const T *e = static_cast<const T *>(event);
      cb(*e);
    };

    subscribers[typeid(T)].emplace_back(wrapper);
  }

 protected:
  Observable();
  virtual ~Observable();

  template <typename T>
  void emit(const T event) {
    auto eventSubscribers = subscribers[typeid(T)];
    for (callback cb : eventSubscribers) cb(&event);
  }

 private:
  using callback = std::function<void(const Event *)>;
  std::unordered_map<std::type_index, std::vector<callback>> subscribers;
};