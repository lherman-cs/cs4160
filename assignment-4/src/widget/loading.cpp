#include "widget/loading.h"
#include "global/global.h"
#include "util/ioMod.h"

Vector2f getTextPos(const std::string &text, int size) {
  const auto c = 0.58;
  const auto topMargin = 60;
  const auto textLength = text.length() * size * c;

  auto screenWidth = Global::get().gamedata.getXmlInt("world/width");
  auto screenHeight = Global::get().gamedata.getXmlInt("world/height");

  auto y = (screenHeight - size) / 2 + topMargin;
  auto x = (screenWidth - textLength) / 2;

  return Vector2f(x, y);
}

Loading::Loading(std::string text, int size)
    : text(text), writer(size), textPos(getTextPos(text, size)) {
  std::cout << "[loading] finished initializing" << std::endl;
  loading.setPosition(Vector2f(470, 300));
}

void Loading::draw() const {
  if (!showing) return;

  modal.draw();
  loading.draw();
  writer.writeText(text, textPos[0], textPos[1], loadingColor);
}

void Loading::update(Uint32 ticks) { loading.update(ticks); }