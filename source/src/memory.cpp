#include "memory.h"
#include "chip8.h"
#include "font.h"
#include "fstream"
#include "iostream"
#include <cstring>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

Memory::Memory(Config *config) : _config(config) {}

Memory::~Memory() {}

void Memory::LoadROM(std::string filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);

  if (file.is_open()) {
    std::streampos size = file.tellg();
    char* buffer = new char[size];

    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    // 0x200 - 0xFFF is used to store the instructions from loaded ROM.
    if (size > (sizeof(this->_memory) - this->_config->romStartAddress)) {
      std::cout << "ROM size is too big and will not fit into memory. Aborting.." << std::endl;
      // Catch this later
      // learn.microsoft.com/en-us/cpp/cpp/errors-and-exception-handling-modern-cpp?view=msvc-170
      throw std::invalid_argument("ROM Size cannot exceed 3584 bytes!");
    }

    // Make this long? Potential for errors
    for (int i = 0; i < size; i++) {
      this->_memory[this->_config->romStartAddress + i] = buffer[i];
    }

    delete[] buffer;
  }
};

void Memory::LoadFont(FontName fontName) {
  Font font(fontName); 

  const uint8_t* fontDataPtr = font.GetBytes();
  std::memcpy(&this->_memory[FONT_START_ADDRESS], fontDataPtr, FontData::FONT_SIZE);
}

uint8_t Memory::GetByte(uint16_t memoryAddr) {
  return this->_memory[memoryAddr];
}

void Memory::SetByte(uint16_t memoryAddr, uint8_t value) {
  this->_memory[memoryAddr] = value;
}

void Memory::DumpMemory() {
  const int bytesPerLine = 16; // Number of bytes per line in the dump

  std::cout << "Dumping " << sizeof(_memory) << " bytes..." << std::endl;
  std::ostringstream out;
  for (int address = 0; address < sizeof(_memory); address += bytesPerLine) {
    out << "| " << std::hex << std::setw(4) << std::setfill('0') << address << " | ";

    for (size_t byte = 0; byte < bytesPerLine; byte++) {
      if (address + byte < sizeof(_memory)) {
        out << std::setw(2) << std::setfill('0') << static_cast<int>(_memory[address + byte]) << ' ';
      } else {
        out << "   ";
      }
    }

    // ASCII
    out << "| ";
    for (int byte = 0; byte < bytesPerLine; byte++) {
      if (address + byte < sizeof(_memory)) {
        char c = _memory[address + byte];
        if (std::isprint(c)) {
          out << c;
        } else {
              out << '.'; // Non printable
        }
      }
    }
    out << " |" << std::endl;
  }


  std::cout << out.str();
}
