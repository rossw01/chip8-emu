#ifndef DISPLAY_H
#define DISPLAY_H

#include "cstdint"

const unsigned int DISPLAY_WIDTH = 64;
const unsigned int DISPLAY_HEIGHT = 32;

class Display {
public:
  Display();
  ~Display();

private:
  uint32_t _screen[DISPLAY_WIDTH * DISPLAY_HEIGHT]{0}; 
};

#endif // !DISPLAY_H
