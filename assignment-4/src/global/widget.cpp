#include "global/widget.h"

WidgetController::WidgetController() : widgets() {
  std::cout << "[widgetController] finished initializing" << std::endl;
}

void WidgetController::draw() const {
  for (const auto w : widgets) {
    if (auto instance = w.lock()) instance->draw();
  }
}

void WidgetController::update(Uint32 ticks) {
  auto it = widgets.begin();
  while (it != widgets.end()) {
    const auto w = it->lock();
    if (!w) {
      it = widgets.erase(it);
      continue;
    }

    w->update(ticks);
    it++;
  }
}