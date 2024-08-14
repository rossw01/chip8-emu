#include "input.h"

Input::Input() {};

Input::~Input() {};

bool Input::IsKeyDown(uint8_t index) {
  return this->_keypad[index];
};
