#include "chip8.h"

Chip8::Chip8() : 
  _config{ FONT_START_ADDRESS, ROM_START_ADDRESS },
  _graphics(),
  _input(&this->_graphics),
  _memory(&this->_config),
  _display(&this->_memory),
  _cpu(&this->_config, &this->_display, &this->_input, &this->_memory) // I think display needs to be a pointer
{
  this->_cpu.Reset();
  this->_memory.LoadFont(DEFAULT);
  this->_memory.DumpMemory();

};

Chip8::~Chip8() {};
