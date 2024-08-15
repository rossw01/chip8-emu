#include "chip8.h"
#include <chrono>
#include <iostream>
#include <ostream>

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " <Scale> <Delay (FPS)> <IsHeadless (1/0)> <ROM Path>" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  int scale = std::stoi(argv[1]);
  int delay = std::stoi(argv[2]);
  bool isHeadless = std::stoi(argv[3]);
  std::string romPath = argv[4];
  
  Chip8 chip8(romPath, scale, delay, isHeadless);

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
