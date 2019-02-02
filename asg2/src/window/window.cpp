#include "window/window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <string.h>
#include "frameGen/frameGenerator.h"

#include <string>

Window::Window(int width, int height, const std::string title)
    : width(width), height(height), title(title) {
  SDL_Init(SDL_INIT_VIDEO);
  window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
}

Window::Window(const Window& other)
    : width(other.width), height(other.height), title(other.title) {
  window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
}

Window::~Window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

Window& Window::operator=(const Window& rhs) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  renderer = rhs.renderer;
  window = rhs.window;
  width = rhs.width;
  height = rhs.height;
  title = rhs.title;
  return *this;
}

void Window::loop(std::shared_ptr<const Shape> root) {
  SDL_Event event;
  const Uint8* keystate;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  root->draw(Renderer(renderer, root->x, root->y, root->w, root->h));
  draw(root, root->x, root->y);
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

void Window::draw(std::shared_ptr<const Shape> root, int x, int y) {
  Renderer _renderer(renderer, x, y, root->w, root->h);

  // maybe check out of bound bounding boxes here
  for (const auto child : root->children) {
    child->draw(_renderer);
    draw(child, x + child->x, y + child->y);
  }
}

void Window::save(std::shared_ptr<const Shape> root, const std::string name) {
  SDL_Surface* bakcground = SDL_LoadBMP("background.bmp");
  SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bakcground);
  SDL_FreeSurface(bakcground);
  SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

  root->draw(Renderer(renderer, root->x, root->y, root->w, root->h));
  draw(root, root->x, root->y);

  TTF_Init();
  TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  SDL_Color textColor = {0xff, 0, 0, 0};
  SDL_Surface* surface = TTF_RenderText_Solid(font, title.c_str(), textColor);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {20, height - 40, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);

  SDL_RenderPresent(renderer);
  FrameGenerator frameGen(renderer, window, width, height, name);
  frameGen.makeFrame();
}
