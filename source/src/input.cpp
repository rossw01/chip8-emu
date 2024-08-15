#include "input.h"
#include <SDL_events.h>
#include <SDL_keycode.h>

Input::Input() {
};

Input::~Input() {};

bool Input::IsKeyDown(uint8_t index) {
  return this->_keypad[index];
};

bool Input::HandleInput() {
  bool quit = false;
  SDL_Event event;

  // DO NOT SCROLL DOWN! NIGHTMARES BEYOND YOUR COMPREHENSION AWAIT YOU! 
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
          case SDLK_ESCAPE:
            quit = true;
            break;
          case SDLK_x:
            this->_keypad[0] = 1;
            break;
          case SDLK_1:
            this->_keypad[1] = 1;
            break;
          case SDLK_2:
            this->_keypad[2] = 1;
            break;
          case SDLK_3:
            this->_keypad[3] = 1;
            break;
          case SDLK_q:
            this->_keypad[4] = 1;
            break;
          case SDLK_w:
            this->_keypad[5] = 1;
            break;
          case SDLK_e:
            this->_keypad[6] = 1;
            break;
          case SDLK_a:
            this->_keypad[7] = 1;
            break;
          case SDLK_s:
            this->_keypad[8] = 1;
            break;
          case SDLK_d:
            this->_keypad[9] = 1;
            break;
          case SDLK_z:
            this->_keypad[0xA] = 1;
            break;
          case SDLK_c:
            this->_keypad[0xB] = 1;
            break;
          case SDLK_4:
            this->_keypad[0xC] = 1;
            break;
          case SDLK_r:
            this->_keypad[0xD] = 1;
            break;
          case SDLK_f:
            this->_keypad[0xE] = 1;
            break;
          case SDLK_v:
            this->_keypad[0xF] = 1;
            break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
          case SDLK_x:
            this->_keypad[0] = 0;
            break;
          case SDLK_1:
            this->_keypad[1] = 0;
            break;
          case SDLK_2:
            this->_keypad[2] = 0;
            break;
          case SDLK_3:
            this->_keypad[3] = 0;
            break;
          case SDLK_q:
            this->_keypad[4] = 0;
            break;
          case SDLK_w:
            this->_keypad[5] = 0;
            break;
          case SDLK_e:
            this->_keypad[6] = 0;
            break;
          case SDLK_a:
            this->_keypad[7] = 0;
            break;
          case SDLK_s:
            this->_keypad[8] = 0;
            break;
          case SDLK_d:
            this->_keypad[9] = 0;
            break;
          case SDLK_z:
            this->_keypad[0xA] = 0;
            break;
          case SDLK_c:
            this->_keypad[0xB] = 0;
            break;
          case SDLK_4:
            this->_keypad[0xC] = 0;
            break;
          case SDLK_r:
            this->_keypad[0xD] = 0;
            break;
          case SDLK_f:
            this->_keypad[0xE] = 0;
            break;
          case SDLK_v:
            this->_keypad[0xF] = 0;
            break;
        }
        break;
        // hey man I tried to warn you
    }
  }
  return quit;
}
