#pragma once
#include "clock.h"
#include "gameData.h"
#include "imageFactory.h"
#include "mixer.h"
#include "navigator.h"
#include "promise.h"
#include "renderContext.h"
#include "viewport.h"
#include "widget.h"

class Global {
 private:
  Global();
  ~Global();
  bool initialized;

 public:
  static Global& get();
  Global(const Global&) = delete;
  Global& operator=(const Global&) = delete;

  Gamedata gamedata;
  Clock clock;
  RenderContext renderContext;
  ImageFactory imageFactory;
  Viewport viewport;
  WidgetController widget;
  Mixer mixer;
  Navigator navigator;
  PromiseScheduler promise;
};