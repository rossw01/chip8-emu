# chip8-emu

This project emulates the functionality of the Chip8 using C++. Graphics and inputs are handled using SDL2.

From [Wikipedia](https://en.wikipedia.org/wiki/CHIP-8): CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker on his 1802 microprocessor. It was initially used on the COSMAC VIP and Telmac 1800, which were 8-bit microcomputers made in the mid-1970s. 

## Requirements
- [SDL2](https://github.com/libsdl-org/SDL/releases/latest)

## Installation
#### Build from source
  - Clone the repo `git clone https://github.com/rossw01/chip8-emu.git`
  - Create build directory and cd into it `cd chip8-emu && mkdir build && cd build`
  - Build and run `cmake .. && make && ./chip8-emu 10 3 0 ../roms/{ROMNAME}`

## Usage
`Usage: ./chip8-emu <scale> <delay> <headless> <rom_path>`

- `(int) scale` determines the sprite scale.
- `(int) delay` determines the time between screen refreshes (lower = faster game speed).
- `(bool) headless` determines whether the SDL2 functionality is initialised.
- `(str) rom_path` determines the location of the ROM to be loaded.

<br />
<p align="center">
  <img src="https://github.com/user-attachments/assets/c046e9c4-aff7-4c54-91a8-ff1b76126d79" />
</p>
<p align="center">
    All CPU Instructions function as expected.
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/e7aa9fe7-cd14-46f8-ab26-337bc65d613e" />
</p>
<p align="center">
    Breakout being played on the emulator.
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/18de43c6-7db6-4b0f-b77d-7e3333c61a70" />
</p>
<p align="center">
    Demonstration of the built-in hex-viewer.
</p>

