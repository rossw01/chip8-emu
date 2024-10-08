#include "font.h"
#include <cstdint>
#include <cstring>

namespace FontData {
  const uint8_t FONT_DEFAULT[FontData::FONT_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
}

const std::unordered_map<FontName, const uint8_t*> Font::_fontMap = {
  {FontName::DEFAULT, FontData::FONT_DEFAULT}
};

Font::Font(FontName fontName) {
  auto it = this->_fontMap.find(fontName);
  if (it != this->_fontMap.end()) {
    std::memcpy(this->_data, it->second, FontData::FONT_SIZE);
  } else {
    std::memcpy(this->_data, this->_fontMap.begin()->second, FontData::FONT_SIZE);
  }
}

const uint8_t* Font::GetBytes() {
  return this->_data;
}
