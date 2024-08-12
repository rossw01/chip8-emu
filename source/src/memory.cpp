#include "memory.h";
#include "fstream"
#include "iostream"
#include <stdexcept>
#include <string>

Memory::Memory(const Config &config) : _config(config) {}

void Memory::LoadROM(std::string filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);

  if (file.is_open()) {
    std::streampos size = file.tellg();
    char* buffer = new char[size];

    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    // 0x200 - 0xFFF is used to store the instructions from loaded ROM.
    if (size > (sizeof(this->_memory) - this->_config.romStartAddress)) {
      std::cout << "ROM size is too big and will not fit into memory. Aborting.." << std::endl;
      // Catch this later
      // learn.microsoft.com/en-us/cpp/cpp/errors-and-exception-handling-modern-cpp?view=msvc-170
      throw std::invalid_argument("ROM Size cannot exceed 3584 bytes!");
    }

    // Make this long? Potential for errors
    for (int i = 0; i < size; i++) {
      this->_memory[this->_config.romStartAddress + i] = buffer[i];
    }

    delete[] buffer;
  }
};
