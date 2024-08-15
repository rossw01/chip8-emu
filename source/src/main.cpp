#include "chip8.h"
#include <chrono>
#include <iostream>
#include <ostream>

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <Scale> <Delay (FPS)> <ROM Path>" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  int scale = std::stoi(argv[1]);
  int delay = std::stoi(argv[2]);
  std::string romPath = argv[3];
  
  Chip8 chip8(romPath, scale, delay);

  auto lastCycle = std::chrono::high_resolution_clock::now();
  bool quit = false;

  while(!quit) {
    auto currTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currTime - lastCycle).count();

    if (dt > delay) {
      lastCycle = currTime;
      quit = chip8.Cycle();
    }
  }

  return 0;
}
