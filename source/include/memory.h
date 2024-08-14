#ifndef MEMORY_H
#define MEMORY_H

#include "cstdint"
#include "string"
#include "config.h"
#include "font.h"

class Memory {
public:
  Memory(Config *config);
  ~Memory();

  void LoadROM(std::string filename);
  void LoadFont(FontName fontName);
  uint8_t GetByte(uint16_t memoryAddr);
  void SetByte(uint16_t memoryAddr, uint8_t value);
  void DumpMemory();

private:
  uint8_t _memory[0x1000]{0}; // 4KiB
  Config *_config;
};

#endif // !MEMORY_H
