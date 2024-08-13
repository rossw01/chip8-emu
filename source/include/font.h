#ifndef FONT_H 
#define FONT_H

#include "cstdint"
#include <unordered_map>

namespace FontData {
  const unsigned int FONT_SIZE = 80;
  extern const uint8_t FONT_DEFAULT[FONT_SIZE];
}

enum FontName {
  DEFAULT,
};

class Font {
public:
  Font(FontName fontName);

  const uint8_t* GetBytes(); 

private:
  uint8_t _data[FontData::FONT_SIZE];
  static const std::unordered_map<FontName, const uint8_t*> _fontMap;

};

#endif // !FONT_H
