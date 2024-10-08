#include "chip8.h"
#include "display.h"

Chip8::Chip8(std::string romPath, int scale, int delay, bool isHeadless): 
  _config{ ROM_START_ADDRESS, FONT_START_ADDRESS },
  _input(),
  _sound(),
  _memory(&this->_config),
  _display(&this->_memory),
  _cpu(&this->_config, &this->_display, &this->_input, &this->_memory, &this->_sound),
  _isHeadless(isHeadless)
{
  if (!this->_isHeadless) {
    this->_graphics = new Graphics("Chip8-Emu", DISPLAY_WIDTH * scale, DISPLAY_HEIGHT * scale, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  };

  this->_cpu.Reset();
  this->_memory.LoadROM(romPath);
  this->_memory.LoadFont(DEFAULT);
  // this->_memory.DumpMemory();
};

Chip8::~Chip8() {
  if (_graphics) {
    delete _graphics;
  }
};

bool Chip8::Cycle() {
  this->_cpu.Cycle();

  if (!this->_isHeadless) {
    this->_graphics->Update(this->_display.GetScreen(), sizeof(uint32_t) * DISPLAY_WIDTH);
  }

  if (this->_input.HandleInput()) {
    this->_graphics->Quit();
    return true; // Quit signal
  }

  return false;
}
