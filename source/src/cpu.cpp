#include "cpu.h"
#include "chip8.h"
#include "display.h"
#include "font.h"
#include <cstdint>
#include <random>

Cpu::Cpu(Config *config, Display *display, Input *input, Memory *memory) : 
  _config(config), _display(display), _input(input), _memory(memory) {}

Cpu::~Cpu() {}

void Cpu::Reset() {
  this->_programCounter = this->_config->romStartAddress;
}

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

  this->_registers[registerX] = (
    this->_registers[registerX] | this->_registers[registerY]
  );
}

void Cpu::OP_8xy2() { // AND Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  this->_registers[registerX] = (
    this->_registers[registerX] & this->_registers[registerY]
  );
}

void Cpu::OP_8xy3() { // XOR Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  this->_registers[registerX] = (
    this->_registers[registerX] ^ this->_registers[registerY]
  );
}

void Cpu::OP_8xy4() { // ADD Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  uint16_t total = this->_registers[registerX] + this->_registers[registerY];

  if (total > 255) {
    this->_registers[0xF] = 1;
  } else {
    this->_registers[0xF] = 0;
  }

  this->_registers[registerX] = total & 0xFF; // Rollover
}

void Cpu::OP_8xy5() { // SUB Vx, Vy
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;


  if (this->_registers[registerX] > this->_registers[registerY]) {
    this->_registers[0xF] = 1;
  } else {
    this->_registers[0xF] = 0;
  }

  this->_registers[registerX] -= this->_registers[registerY];
}

void Cpu::OP_8xy6() { // SHR Vx {, Vy}
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;

  this->_registers[0xF] = this->_registers[registerX] & 0x1;
  this->_registers[registerX] >>= 1;
}


void Cpu::OP_8xy7() { // SHR Vx {, Vy}
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;
  uint8_t registerY = (this->_opcode & 0x00F0) >> 4;

  if (this->_registers[registerY] > this->_registers[registerX]) {
    this->_registers[0xF] = 1;
  } else {
    this->_registers[0xF] = 0;
  }

  this->_registers[registerX] = (
   this->_registers[registerY] - this->_registers[registerX]
  );
}

void Cpu::OP_8xyE() { // SHL Vx {, Vy} 
  uint8_t registerX = (this->_opcode & 0x0F00) >> 8;

  this->_registers[0xF] = (this->_registers[registerX] & 0b10000000) >> 7;
  this->_registers[registerX] <<= 2;
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
      registerX % DISPLAY_WIDTH, registerY % DISPLAY_HEIGHT, 
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
