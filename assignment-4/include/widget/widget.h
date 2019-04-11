#pragma once
#include <functional>
#include "core/interface.h"
#include "core/promise.h"

class Widget : public Drawable, public Updatable {
 private:
  bool _show() {
    showing = true;
    return true;
  }
  bool _dismiss() {
    showing = false;
    return true;
  }

 protected:
  bool showing = false;

 public:
  std::function<bool()> show = std::bind(&Widget::_show, this);
  std::function<bool()> dismiss = std::bind(&Widget::_dismiss, this);
};