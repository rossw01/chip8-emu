#ifndef INPUT_H
#define INPUT_H

#include "cstdint"
#include <SDL_keycode.h>
#include <unordered_map>

class Input {
public:
  Input();
  ~Input();

  bool IsKeyDown(uint8_t index);
  bool HandleInput();

private:
  static const std::unordered_map<SDL_Keycode, int> _keyMap;
  bool _keys[16]{false};

};

#endif // !INPUT_H
