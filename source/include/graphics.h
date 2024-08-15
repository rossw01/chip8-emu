#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "display.h"
#include <SDL_render.h>

namespace GraphicsConfig {
  const int SCALE_FACTOR = 4;
  const int WIN_WIDTH = DISPLAY_WIDTH * SCALE_FACTOR;
  const int WIN_HEIGHT = DISPLAY_HEIGHT * SCALE_FACTOR;
}

class Graphics {
public:
  Graphics();
  ~Graphics();

  void Update(void* buffer, int pitch);
  void Quit();

private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  SDL_Texture* _texture;

};

#endif // !GRAPHICS_H
