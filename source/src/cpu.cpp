#include "cpu.h"

Cpu::Cpu(const Config &config ) : _config(config){}

Cpu::~Cpu() {}

void Cpu::Reset() {
  this->_programCounter = this->_config.romStartAddress;
}
