#include "sound.h"
#include <SDL.h>
#include <SDL_audio.h>
#include <chrono>
#include <iostream>
#include <ostream>
#include <thread>

Sound::Sound() : _duration(0), _frequency(440), _volume(0.25f) {
  SDL_Init(SDL_INIT_AUDIO);
  SDL_zero(this->_spec);
  this->_spec.freq = 48000;
  this->_spec.format = AUDIO_S16SYS;
  this->_spec.channels = 1;
  this->_spec.samples = 4096;
  this->_spec.callback = Sound::callbackWrapper;
  this->_spec.userdata = this;

  if ((id = SDL_OpenAudioDevice(NULL, 0, &this->_spec, &this->_aspec, SDL_AUDIO_ALLOW_ANY_CHANGE)) <= 0) {
    std::cerr << "Audio device couldn't be opened: " << SDL_GetError() << std::endl;
    std::exit(-1);
  }
}

Sound::~Sound() {
  SDL_CloseAudioDevice(id);
  SDL_Quit();
}

void Sound::PlaySound() {
  SDL_PauseAudioDevice(id, 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  SDL_PauseAudioDevice(id, 1);
}

void Sound::callbackWrapper(void *userdata, uint8_t *stream, int len) {
  Sound *sound = reinterpret_cast<Sound*>(userdata);
  sound->callback(stream, len);
}

void Sound::callback(uint8_t *stream, int len) {
  const float PI_2 = 6.283185;

  // idk what this does i cant lie
  short *snd = reinterpret_cast<short*>(stream); 
  len /= sizeof(*snd);
  for (int i = 0; i < len; i++) {
    snd[i] = 32000 * this->_volume * std::sin(this->_duration);
    this->_duration += this->_frequency * PI_2 / this->_spec.freq;
    if (this->_duration >= PI_2) {
      this->_duration -= PI_2;
    }
  }
}
