#include "chip8.h"

Chip8::Chip8() : 
  _config{ FONT_START_ADDRESS, ROM_START_ADDRESS },
  _cpu(this->_config),
  _memory(this->_config),
  _input(),
  _display()
{
  this->_cpu.Reset();
  this->_memory.LoadFont(DEFAULT);
  this->_memory.DumpMemory();
};

Chip8::~Chip8() {};
