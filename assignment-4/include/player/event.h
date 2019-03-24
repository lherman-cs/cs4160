#pragma once
#include <cstdint>
#include <functional>

class Event {
 public:
  virtual ~Event();
};

class CallLiarEvent : public Event {};

class QuitEvent : public Event {};

class RestartEvent : public Event {};

class SetBetEvent : public Event {
 public:
  SetBetEvent(uint64_t quantity, uint64_t type)
      : quantity(quantity), type(type) {}
  const uint64_t quantity, type;
};

template <typename T>
using EventCallback = std::function<void(const T&)>;