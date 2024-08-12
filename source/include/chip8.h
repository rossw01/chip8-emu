#ifndef CHIP8_H
#define CHIP8_H

#include "string"
#include "cpu.h"
#include "memory.h"
#include "input.h"
#include "display.h"

const unsigned int ROM_START_ADDRESS = 0x200; // 512 bytes offset

class Chip8 {
public:
  Chip8();
  ~Chip8();

private:
  Cpu _cpu;
  Memory _memory;
  Input _input;
  Display _display;
  Config _config;

};


#endif // !CHIP8
