#ifndef SOUND_H
#define SOUND_H

#include <SDL_audio.h>
class Sound {
public:
  Sound();
  ~Sound();
  void PlaySound();

private:
  SDL_AudioSpec _spec, _aspec;
  int id;
  float _duration;
  float _frequency;
  float _volume;

  static void callbackWrapper(void *userdata, uint8_t *stream, int len);
  void callback(uint8_t *stream, int len);

};

#endif // !SOUND_H
