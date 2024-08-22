#include "input.h"
#include <SDL_events.h>
#include <SDL_keycode.h>

const std::unordered_map<SDL_Keycode, int> Input::_keyMap = {
  {SDLK_1, 1}, {SDLK_2, 2}, {SDLK_3, 3}, {SDLK_4, 0xC},
  {SDLK_q, 4}, {SDLK_w, 5}, {SDLK_e, 6}, {SDLK_r, 0xD},
  {SDLK_a, 7}, {SDLK_s, 8}, {SDLK_d, 9}, {SDLK_f, 0xE},
  {SDLK_z, 0xA}, {SDLK_x, 0}, {SDLK_c, 0xB}, {SDLK_v, 0xF}
};

Input::Input() : _keys{false} {};

Input::~Input() {};

bool Input::IsKeyDown(uint8_t index) {
  return this->_keys[index];
};

bool Input::HandleInput() { // Returning true value starts quit sequence
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return true;
    }
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
      auto it = this->_keyMap.find(event.key.keysym.sym);
      if (it == this->_keyMap.end()) {
        return false;
      }
      this->_keys[it->second] = event.type == SDL_KEYDOWN ? true : false;
    }
  }
  return false;
}
