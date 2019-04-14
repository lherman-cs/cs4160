#pragma once
#include <functional>
#include <memory>
#include "core/interface.h"
#include "sprite/multisprite.h"
#include "sprite/sprite.h"
#include "util/ioMod.h"

/* ignore effc++ since enabled_from_this is meant to be created using smart
 * pointers */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

class Widget : public Drawable,
               public Updatable,
               public std::enable_shared_from_this<Widget> {
 private:
  static unsigned long long cntr;
  void _show() { showing = true; }
  void _dismiss() { showing = false; }

 protected:
  bool showing = false;

 public:
  std::function<bool()> show() {
    auto f = std::bind(&Widget::_show, this->shared_from_this());
    return [f]() -> bool {
      f();
      return true;
    };
  }
  std::function<bool()> dismiss() {
    auto f = std::bind(&Widget::_dismiss, this->shared_from_this());
    return [f]() -> bool {
      f();
      return true;
    };
  }
};

#pragma GCC diagnostic pop