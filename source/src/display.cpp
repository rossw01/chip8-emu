#include "display.h"
#include <cstring>

Display::Display(Memory *memory): _memory(memory) { }

Display::~Display() { }

void Display::ClearScreen() {
  std::memset(&this->_screen, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT);
}

// If drawing has wrapped, return true
bool Display::Draw(uint8_t posX, uint8_t posY, uint8_t spriteHeight, uint16_t index) {
  bool didErasePixel = false;

  for (int row = 0; row < spriteHeight; row++) {
    // Index holds an address pointing to sprite data
    uint8_t spriteByte = this->_memory->GetByte(index + row);

    for (int col = 0; col < 8; col++) {
      uint8_t spritePixel = spriteByte & (0b10000000 >> col);
      uint32_t* screenPixel = &this->_screen[((posY + row) * DISPLAY_WIDTH) + (posX + col)];
      if (!spritePixel) {
        continue;
      }
      if (*screenPixel == 0xFFFFFFF) {
        didErasePixel = true;
      }
      *screenPixel ^= 0xFFFFFFFF;
    }
  }
  return didErasePixel;
}

uint32_t* Display::GetScreen() {
  return this->_screen;
}
