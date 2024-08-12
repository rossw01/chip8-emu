#ifndef CPU_H
#define CPU_H

#include "config.h"
#include "cstdint"

class Cpu {
public:
  Cpu(const Config &config);
  ~Cpu();

  void Reset();

private:
  uint16_t _opcode;
  uint8_t _registers[16]{0};
  uint16_t _index; // 16bit index register
  uint16_t _programCounter; // 16bit program counter
  uint16_t _callStack[16]{0}; // 16 depth, 16bit call stack
  uint8_t _stackPointer{}; // 8bit stack pointer
  uint8_t _delayTimer{}; // If not 0, decrements at 60hz
  uint8_t _soundTimer{}; // Same as delay, buzzer beeps when 0

  Config _config;
};

#endif // !CPU_H
