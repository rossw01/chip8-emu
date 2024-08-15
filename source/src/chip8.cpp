#include "chip8.h"
#include "display.h"

Chip8::Chip8(std::string romPath, int scale, int delay ) : 
  _config{ ROM_START_ADDRESS, FONT_START_ADDRESS },
  _graphics("Chip8-Emu", DISPLAY_WIDTH * scale, DISPLAY_HEIGHT * scale, DISPLAY_WIDTH, DISPLAY_HEIGHT),
  _input(),
  _memory(&this->_config),
  _display(&this->_memory),
  _cpu(&this->_config, &this->_display, &this->_input, &this->_memory) 
{
  this->_cpu.Reset();
  this->_memory.LoadROM(romPath);
  // this->_memory.LoadFont(DEFAULT);
  // this->_memory.DumpMemory();
};

Chip8::~Chip8() {};

bool Chip8::Cycle() {
  this->_cpu.Cycle();
  this->_graphics.Update(this->_display.GetScreen(), sizeof(uint32_t) * DISPLAY_WIDTH);
  
  if (this->_input.HandleInput()) {
    this->_graphics.Quit();
    return false; // Quit signal
  }

  return true;
}

Input* Chip8::GetInput() {
  return &this->_input;
}
