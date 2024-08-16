#include "cpu.h"
#include "chip8.h"
#include "display.h"
#include "font.h"
#include <cstdint>
#include <iostream>
#include <random>

Cpu::Cpu(Config *config, Display *display, Input *input, Memory *memory) : 
  _config(config), _display(display), _input(input), _memory(memory) {
  this->_table[0x0] = &Cpu::Table0;
  this->_table[0x1] = &Cpu::OP_1nnn;
  this->_table[0x2] = &Cpu::OP_2nnn;
  this->_table[0x3] = &Cpu::OP_3xkk;
  this->_table[0x4] = &Cpu::OP_4xkk;
  this->_table[0x5] = &Cpu::OP_5xy0;
  this->_table[0x6] = &Cpu::OP_6xkk;
  this->_table[0x7] = &Cpu::OP_7xkk;
  this->_table[0x8] = &Cpu::Table8;
  this->_table[0x9] = &Cpu::OP_9xy0;
  this->_table[0xA] = &Cpu::OP_Annn;
  this->_table[0xB] = &Cpu::OP_Bnnn;
  this->_table[0xC] = &Cpu::OP_Cxkk;
  this->_table[0xD] = &Cpu::OP_Dxyn;
  this->_table[0xE] = &Cpu::TableE;
  this->_table[0xF] = &Cpu::TableF;

  // Populate function ptr subtables
  for (size_t i = 0; i < 0xE; i++) {
    this->_table0[i] = &Cpu::Void;
    this->_table8[i] = &Cpu::Void;
    this->_tableE[i] = &Cpu::Void;
  }
  for (size_t i = 0; i < 0x65; i++) {
    this->_tableF[i] = &Cpu::Void;
  }

  this->_table0[0x0] = &Cpu::OP_00E0;
  this->_table0[0xE] = &Cpu::OP_00EE;

  this->_table8[0x0] = &Cpu::OP_8xy0;
  this->_table8[0x1] = &Cpu::OP_8xy1;
  this->_table8[0x2] = &Cpu::OP_8xy2;
  this->_table8[0x3] = &Cpu::OP_8xy3;
  this->_table8[0x4] = &Cpu::OP_8xy4;
  this->_table8[0x5] = &Cpu::OP_8xy5;
  this->_table8[0x6] = &Cpu::OP_8xy6;
  this->_table8[0x7] = &Cpu::OP_8xy7;
  this->_table8[0xE] = &Cpu::OP_8xyE;

  this->_tableE[0xE] = &Cpu::OP_Ex9E;
  this->_tableE[0x1] = &Cpu::OP_ExA1;

  this->_tableF[0x07] = &Cpu::OP_Fx07;
  this->_tableF[0x0A] = &Cpu::OP_Fx0A;
  this->_tableF[0x15] = &Cpu::OP_Fx15;
  this->_tableF[0x18] = &Cpu::OP_Fx18;
  this->_tableF[0x1E] = &Cpu::OP_Fx1E;
  this->_tableF[0x29] = &Cpu::OP_Fx29;
  this->_tableF[0x33] = &Cpu::OP_Fx33;
  this->_tableF[0x55] = &Cpu::OP_Fx55;
  this->_tableF[0x65] = &Cpu::OP_Fx65;
}

Cpu::~Cpu() {}


void Cpu::Cycle() {
  // Combines two bytes and combines them to make our 16 bit opcode
  this->_opcode = this->_memory->GetByte(this->_programCounter) << 8 | 
                  this->_memory->GetByte(this->_programCounter + 1);
  this->_programCounter += 2;

  ((*this).*(this->_table[(this->_opcode & 0xF000u) >> 12]))();

  if (this->_delayTimer > 0) {
    this->_delayTimer--;
  }

  if (this->_soundTimer > 0) {
    this->_soundTimer--;
  }
}

void Cpu::Reset() {
  this->_programCounter = this->_config->romStartAddress;
}

void Cpu::Table0() {
  (this->*_table0[this->_opcode & 0x000F])();
  // ((*this).*(this->_table0[this->_opcode & 0x000F]))(); // lol
}

void Cpu::Table8() {
  ((*this).*(this->_table8[this->_opcode & 0x000F]))();
}

void Cpu::TableE() {
  ((*this).*(this->_tableE[this->_opcode & 0x000F]))();
}

void Cpu::TableF() {
  // never forget the hour spent trying to track down this bug. I used 0x000F 
  // as the mask like I did with the other tables... big mistake...
  ((*this).*(this->_tableF[this->_opcode & 0x00FF]))();
}

void Cpu::Void() {}

void Cpu::OP_00E0() { // CLS
  this->_display->ClearScreen();
}

void Cpu::OP_00EE() { // RET
  this->_stackPointer -= 1;
  this->_programCounter = this->_callStack[this->_stackPointer];
}

void Cpu::OP_1nnn() { // JP addr
  uint16_t address = this->_opcode & 0x0FFF; // Set leading 4 bits to 0
  this->_programCounter = address;
}

void Cpu::OP_2nnn() { // Call addr
  uint16_t address = this->_opcode & 0x0FFF; // Set leading 4 bits to 0
  this->_callStack[_stackPointer] = this->_programCounter;
  this->_stackPointer++;
  this->_programCounter = address;
}

void Cpu::OP_3xkk() { // SE Vx, byte
  uint8_t registerIndex = (this->_opcode & 0x0F00) >> 8;
  uint8_t compareByte = (this->_opcode & 0x00FF);
  if (this->_registers[registerIndex] == compareByte) {
    this->_programCounter += 2;
  } 
}

void Cpu::OP_4xkk() { // SNE Vx, byte
  uint8_t registerIndex = (this->_opcode & 0x0F00) >> 8;
  uint8_t compareByte = (this->_opcode & 0x00FF);
  if (this->_registers[registerIndex] != compareByte) {
    this->_programCounter += 2;
  } 
}

void Cpu::OP_5xy0() { // SE Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;
  if (this->_registers[registerX] == this->_registers[registerY]) {
    this->_programCounter += 2;
  } 
}

void Cpu::OP_6xkk() { // LD Vx, byte
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t value = (this->_opcode & 0x00FF);

  this->_registers[registerX] = value;
}

void Cpu::OP_7xkk() { // ADD Vx, byte
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t value = (this->_opcode & 0x00FF);

  this->_registers[registerX] += value;
}


void Cpu::OP_8xy0() { // LD Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  this->_registers[registerX] = this->_registers[registerY];
}

void Cpu::OP_8xy1() { // OR Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  this->_registers[registerX] |= this->_registers[registerY];
}

void Cpu::OP_8xy2() { // AND Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  this->_registers[registerX] &= this->_registers[registerY];
}

void Cpu::OP_8xy3() { // XOR Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  this->_registers[registerX] ^= this->_registers[registerY];
}

void Cpu::OP_8xy4() { // ADD Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  uint16_t total = this->_registers[registerX] + this->_registers[registerY];

  uint8_t newVf;
  if (total > 255) {
    newVf = 1;
  } else {
    newVf = 0;
  }

  this->_registers[registerX] = total & 0xFF; // Rollover
  this->_registers[0xF] = newVf; // See explanation in OP_8xy5
}

void Cpu::OP_8xy5() { // SUB Vx, Vy
  // The documentation for 8xy5 and 8xy7 on devernay.free.fr is just incorrect.
  // You must subtract FIRST to allow for Vf to be able to be stored in Vx.
  // Imagine Vx is Vf, you set Vx(Vf) to 1 or 0 to say if we rolled over to 255,
  // then IMMEDIATELY overwrite it by subtracting Vy from it?? obviously fucking wrong.

  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  uint16_t total = this->_registers[registerX] - this->_registers[registerY];

  uint8_t newVf;
  if (this->_registers[registerX] >= this->_registers[registerY]) {
    newVf = 1;
  } else {
    newVf = 0;
  }

  this->_registers[registerX] = total & 0xFF;
  this->_registers[0xF] = newVf; 
}

// FIXME
void Cpu::OP_8xy6() { // SHR Vx {, Vy}
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;

  this->_registers[0xF] = this->_registers[registerX] & 0x1;
  this->_registers[registerX] >>= 1;
}


void Cpu::OP_8xy7() { // SUBN Vx {, Vy}
  uint8_t registerX = (this->_opcode & 0x0F00u) >> 8u;
  uint8_t registerY = (this->_opcode & 0x00F0u) >> 4u;

  uint16_t total = this->_registers[registerY] - this->_registers[registerX];

  uint8_t newVf;
  if (this->_registers[registerY] >= this->_registers[registerX]) {
    newVf = 1;
  } else {
    newVf = 0;
  }

  this->_registers[registerX] = total; 
  this->_registers[0xF] = newVf;
}

// FIXME
void Cpu::OP_8xyE() { // SHL Vx {, Vy} 
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;

  this->_registers[0xF] = (this->_registers[registerX] & 0b10000000) >> 7;
  this->_registers[registerX] <<= 1;
}

void Cpu::OP_9xy0() { // SNE Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  if (this->_registers[registerX] != this->_registers[registerY]) {
    this->_programCounter += 2;
  }
}

void Cpu::OP_Annn() { // LD I, addr
  this->_index = this->_opcode & 0x0FFF;
}

void Cpu::OP_Bnnn() { // JP V0, addr
  uint16_t destAddress = this->_opcode & 0x0FFF;
  this->_programCounter = destAddress + this->_registers[0x0];
}

void Cpu::OP_Cxkk() { // RND Vx, byte
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t value = this->_opcode & 0x00FF;

  std::random_device rd;
  std::default_random_engine rng(rd());
  std::uniform_int_distribution<uint8_t> random(0, 255);
  uint8_t randomByte = random(rng);

  this->_registers[registerX] = value & randomByte;
}

void Cpu::OP_Dxyn() { // DRW Vx, Vy, nibble
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;
  uint8_t spriteHeight = this->_opcode & 0x000F;

  this->_registers[0xF] = (
    this->_display->Draw(
      this->_registers[registerX] % DISPLAY_WIDTH, this->_registers[registerY] % DISPLAY_HEIGHT, 
      spriteHeight, this->_index
    )
  );
}

void Cpu::OP_Ex9E() { // SKP Vx
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  if (this->_input->IsKeyDown(registerX)) {
    this->_programCounter += 2;
  }
}

void Cpu::OP_ExA1() { // SKNP Vx
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  if (!this->_input->IsKeyDown(registerX)) {
    this->_programCounter += 2;
  }
}

void Cpu::OP_Fx07() { // LD Vx, DT
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  this->_registers[registerX] = this->_delayTimer;
}

void Cpu::OP_Fx0A() { // LD Vx, K
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  for (uint8_t i = 0; i < 16; i++) {
    if (this->_input->IsKeyDown(i)) {
      this->_registers[registerX] = i;
      return;
    }
  }
  this->_programCounter -= 2;
}

void Cpu::OP_Fx15() { // LD DT, Vx
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  this->_delayTimer = this->_registers[registerX];
}

void Cpu::OP_Fx18() { // LD ST, Vx
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  this->_soundTimer = this->_registers[registerX];
}

void Cpu::OP_Fx1E() { // ADD I, Vx
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  this->_index += this->_registers[registerX];
}

void Cpu::OP_Fx29() { // LD F, Vx
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t spriteIndex = this->_registers[registerX];

  this->_index = FONT_START_ADDRESS + ((FontData::FONT_SIZE / 16) * spriteIndex);
}

void Cpu::OP_Fx33() { // LD B, Vx
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t val = this->_registers[registerX];

  this->_memory->SetByte(this->_index + 2, val % 10);
  val /= 10;
  this->_memory->SetByte(this->_index + 1, val % 10);
  val /= 10;
  this->_memory->SetByte(this->_index, val % 10);
}

void Cpu::OP_Fx55() { // LD [I], Vx
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  for (uint8_t i = 0; i <= registerX; i++) {
    this->_memory->SetByte(this->_index + i, this->_registers[i]);
  }
}

void Cpu::OP_Fx65() { // LD Vx, [I]
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  for (uint8_t i = 0; i <= registerX; i++) {
    this->_registers[i] = this->_memory->GetByte(this->_index + i);
  }
}
