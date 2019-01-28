#include "window/window.h"

Window::Window(int width, int height) : width(width), height(height) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
}

Window::~Window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Window::loop(const Shape& root) {
  SDL_Event event;
  const Uint8* keystate;

  root.draw(Renderer(renderer, root.x, root.y, root.w, root.h));
  draw(root, root.x, root.y);
  SDL_RenderPresent(renderer);

  while (true) {
    keystate = SDL_GetKeyboardState(0);
    if (keystate[SDL_SCANCODE_ESCAPE]) {
      break;
    }
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }
  }
}

void Window::draw(const Shape& root, int x, int y) {
  Renderer _renderer(renderer, x, y, root.w, root.h);

  // maybe check out of bound bounding boxes here
  for (const auto& child : root.children) {
    child->draw(_renderer);
    draw(*child, x + child->x, y + child->y);
  }
}