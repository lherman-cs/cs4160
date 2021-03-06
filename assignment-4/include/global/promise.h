#pragma once
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <queue>
#include "core/interface.h"

class Engine;
class Global;

class Promise : public std::enable_shared_from_this<Promise> {
 private:
  friend class PromiseScheduler;
  // return true if resolved
  using Action = std::function<bool()>;
  std::queue<Action> actions;
  Uint32 elapsed = 0;
  // return false if there's no more actions
  bool next(Uint32 ticks);
  Promise() : actions() {}

 public:
  Promise(const Promise&) = delete;
  Promise& operator=(const Promise&) = delete;
  Promise& then(Action);
  Promise& sleep(uint32_t ms);
};

class PromiseScheduler {
 private:
  friend class Engine;
  friend class Global;
  std::list<std::shared_ptr<Promise>> promises;
  PromiseScheduler() : promises() {}
  void update(Uint32 ticks);

 public:
  PromiseScheduler(const PromiseScheduler&) = delete;
  PromiseScheduler& operator=(const PromiseScheduler&) = delete;
  Promise& add();
};