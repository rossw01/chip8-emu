#ifndef DISPLAY_H
#define DISPLAY_H

#include "cstdint"
#include "memory.h"
#include <cstdint>

const unsigned int DISPLAY_WIDTH = 64;
const unsigned int DISPLAY_HEIGHT = 32;

class Display {
public:
  Display(Memory *memory);
  ~Display();

  void ClearScreen();
  bool Draw(uint8_t posX, uint8_t posY, uint8_t size, uint16_t index);
  uint32_t* GetScreen();

private:
  uint32_t _screen[DISPLAY_WIDTH * DISPLAY_HEIGHT]{0}; 
  Memory *_memory;

};

#endif // !DISPLAY_H
