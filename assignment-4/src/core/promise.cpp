#include "core/promise.h"
#include <memory>

PromiseScheduler& PromiseScheduler::getInstance() {
  static PromiseScheduler instance;
  return instance;
}

std::shared_ptr<PromiseScheduler::Promise> PromiseScheduler::add() {
  auto promise = std::make_shared<Promise>();
  promises.push_back(promise);
  return promise;
}

void PromiseScheduler::update(Uint32 ticks) {
  auto it = promises.begin();
  while (it != promises.end()) {
    if ((*it)->next(ticks)) {
      it++;
      continue;
    }
    it = promises.erase(it);
  }
}

PromiseScheduler::Promise& PromiseScheduler::Promise::then(Action a) {
  actions.push(a);
  return *this;
}

PromiseScheduler::Promise& PromiseScheduler::Promise::sleep(uint32_t seconds) {
  auto a = [&, seconds]() -> bool {
    // elapsed is in miliseconds
    return elapsed > seconds * 1000;
  };
  actions.push(a);
  return *this;
}

bool PromiseScheduler::Promise::next(Uint32 ticks) {
  if (actions.empty()) return false;
  elapsed += ticks;
  if (actions.front()()) {
    actions.pop();
    elapsed = 0;
  }
  return true;
}