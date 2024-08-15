#ifndef CHIP8_H
#define CHIP8_H

#include "cpu.h"
#include "graphics.h"
#include "memory.h"
#include "input.h"
#include "display.h"

const unsigned int FONT_START_ADDRESS = 0x50; // 80 byte offset
const unsigned int ROM_START_ADDRESS = 0x200; // 512 byte offset

class Chip8 {
public:
  Chip8(std::string romPath, int scale, int delay);
  ~Chip8();

  bool Cycle();
  Input* GetInput();

private:
  Cpu _cpu;
  Memory _memory;
  Input _input;
  Display _display;
  Config _config;
  Graphics _graphics;

};

#endif // !CHIP8
