#include "./graphics.h"
#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>

Graphics::Graphics(std::string title, int winWidth, int winHeight, int textureWidth, int textureHeight) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(
    winWidth, winHeight, SDL_WINDOW_SHOWN, &this->_window, &this->_renderer
  );
  this->_texture = SDL_CreateTexture(
    this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
    textureWidth, textureHeight
  );
}

Graphics::~Graphics() {}

void Graphics::Quit() {
  SDL_DestroyTexture(this->_texture);
  SDL_DestroyRenderer(this->_renderer);
  SDL_DestroyWindow(this->_window);
  SDL_Quit();
}

void Graphics::Update(void* buffer, int pitch) {
  SDL_UpdateTexture(this->_texture, nullptr, buffer, pitch);
  SDL_RenderClear(this->_renderer);
  SDL_RenderCopy(this->_renderer, this->_texture, nullptr, nullptr);
  SDL_RenderPresent(this->_renderer);
}

