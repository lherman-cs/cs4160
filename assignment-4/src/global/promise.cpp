#include "global/promise.h"
#include <memory>

Promise& PromiseScheduler::add() {
  std::shared_ptr<Promise> promise(new Promise());
  promises.push_back(promise);
  return *promise;
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

Promise& Promise::then(Action a) {
  actions.push(a);
  return *this;
}

Promise& Promise::sleep(uint32_t seconds) {
  auto a = [&, seconds]() -> bool {
    // elapsed is in miliseconds
    return elapsed > seconds * 1000;
  };
  actions.push(a);
  return *this;
}

bool Promise::next(Uint32 ticks) {
  if (actions.empty()) return false;
  elapsed += ticks;
  if (actions.front()()) {
    actions.pop();
    elapsed = 0;
  }
  return true;
}