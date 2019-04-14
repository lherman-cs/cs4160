#pragma once
#include <list>
#include <memory>
#include "core/interface.h"
#include "widget/widget.h"

class Global;

class WidgetController : public Drawable, public Updatable {
 private:
  friend class Global;
  WidgetController();
  WidgetController(const WidgetController&) = delete;
  WidgetController& operator=(const WidgetController&) = delete;
  std::list<std::weak_ptr<Widget>> widgets;

 public:
  template <typename T,
            typename = typename std::enable_if<
                std::is_base_of<Widget, T>::value, T>::type,
            class... Args>
  std::shared_ptr<T> create(Args&&... args) {
    std::shared_ptr<T> t(new T(args...));
    widgets.push_back(t);
    return t;
  }

  virtual void draw() const;
  virtual void update(Uint32 ticks);
};
