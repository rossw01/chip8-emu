#include "chip8.h"
#include "options.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <set>
#include <stdexcept>
#include <thread>

void invalidUsage(const char* name) {
  std::cerr << "Usage: " << name << " <ROM Path> [options]" << std::endl;
  std::cerr << "Enter '" << name << " --help' for more information." << std::endl;
}

void printHelp(const char* name) {
  std::cerr << "Usage: " << name << "<ROM Path> [options]" << std::endl;
  std::cerr << "Options:" << std::endl;
  std::cerr << "   -hl, -headless: Run the emulator in headless mode (no GUI)." << std::endl;
  std::cerr << "   -s (SCALE_FACTOR), -scale (SCALE_FACTOR): Set the scale factor. Must be followed by an integer." << std::endl;
  std::cerr << "   -d (FRAME_DELAY), -delay (FRAME_DELAY): Set the delay between frames in milliseconds. Must be followed by an integer." << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: No arguments were provided." << std::endl;
    invalidUsage(argv[0]);
    std::exit(EXIT_FAILURE);
  }

  Options options;
  const std::set<std::string> headless_flags = {"-hl", "-headless", "--hl", "--headless"};
  const std::set<std::string> scale_flags = {"-s", "-scale", "--s", "--scale"};
  const std::set<std::string> delay_flags = {"-d", "-delay", "--d", "--delay"};
  const std::set<std::string> help_flags = {"-h", "-help", "--h", "--help"};

  for (int i = 2; i < argc; i++) {
    std::string arg = argv[i];
    std::transform(arg.begin(), arg.end(), arg.begin(), [](unsigned char c){ return std::tolower(c); });

    if (headless_flags.find(arg) != headless_flags.end()) {
      options.isHeadless = true;
    } else if (scale_flags.find(arg) != scale_flags.end()) {
      if (i + 1 >= argc) {
        std::cerr << "Error: Invalid scale argument. Expected a scale factor to be specified after scale flag." << std::endl;
        invalidUsage(argv[0]);
        std::exit(EXIT_FAILURE);
      }
      try {
        options.scale = std::stoi(argv[++i]);
      } catch (const std::invalid_argument &e) {
        std::cerr << "Error: Invalid scale argument. Expected a scale factor to be specified after scale flag." << std::endl;
        invalidUsage(argv[0]);
        std::exit(EXIT_FAILURE);
      }
    } else if (delay_flags.find(arg) != delay_flags.end()) {
      if (i + 1 >=  argc) {
        std::cerr << "Error: Invalid delay argument. Expected a frame delay to be specified after delay flag." << std::endl;
        invalidUsage(argv[0]);
        std::exit(EXIT_FAILURE);
      }
      try {
        options.delay = std::stoi(argv[++i]);
      } catch (const std::invalid_argument &e) {
        std::cerr << "Error: Invalid delay argument. Expected a frame delay to be specified after delay flag." << std::endl;
        invalidUsage(argv[0]);
        std::exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "Error: Unknown option was specified" << std::endl;
      invalidUsage(argv[0]);
      std::exit(EXIT_FAILURE);
    }
  }

  std::string romPath = argv[1];
  std::transform(romPath.begin(), romPath.end(), romPath.begin(), [](unsigned char c){ return std::tolower(c); });

  if (help_flags.find(romPath) != help_flags.end()) { // this is sooo shit
    printHelp(argv[0]);
    std::exit(EXIT_SUCCESS);
  }

  std::filesystem::path pathToValidate(romPath);

  if (!std::filesystem::exists(pathToValidate)) {
    std::cerr << "Error: The provided rom path is invalid (the file could not be found)." << std::endl;
    invalidUsage(argv[0]);
    std::exit(EXIT_FAILURE);
  }
  if (!std::filesystem::is_regular_file(pathToValidate)) {
    std::cerr << "Error: The provided rom file is invalid and cannot be read." << std::endl;
    invalidUsage(argv[0]);
    std::exit(EXIT_FAILURE);
  }

  Chip8 chip8(romPath, options.scale, options.delay, options.isHeadless);

  auto lastCycle = std::chrono::high_resolution_clock::now();
  bool quit = false;

  while (!quit) {
    auto currTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currTime - lastCycle).count();

    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(options.delay - dt)));

    lastCycle = std::chrono::high_resolution_clock::now();
    quit = chip8.Cycle();
  }

  return 0;
}
