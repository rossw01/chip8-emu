#ifndef INPUT_H
#define INPUT_H

#include "cstdint"

class Input {
public:
  Input();
  ~Input();

  bool IsKeyDown(uint8_t index);
  bool HandleInput();

private:
  uint8_t _keypad[16]{0}; 
};

#endif // !INPUT_H
