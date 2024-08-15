#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL_render.h>
#include <string>

class Graphics {
public:
  Graphics(std::string title, int winWidth, int winHeight, int textureWidth, int textureHeight);
  ~Graphics();

  void Update(void* buffer, int pitch);
  void Quit();

private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  SDL_Texture* _texture;

};

#endif // !GRAPHICS_H
