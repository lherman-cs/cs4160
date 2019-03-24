#pragma once
#include <cstdint>
#include <functional>

class Event {
 public:
  virtual ~Event();
};

template <class T, typename = typename std::enable_if<
                       std::is_base_of<Event, T>::value, T>::type>
using EventCallback = std::function<void(const T&)>;