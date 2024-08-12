#include "chip8.h"

Chip8::Chip8() : 
  _config{ROM_START_ADDRESS},
  _cpu(this->_config),
  _memory(this->_config),
  _input(),
  _display()
{
  this->_cpu.Reset();
};

Chip8::~Chip8() {};
