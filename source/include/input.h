#ifndef INPUT_H
#define INPUT_H

#include "cstdint"

class Input {
public:
  Input();
  ~Input();

private:
  uint8_t _keypad[16]; 
};

#endif // !INPUT_H
