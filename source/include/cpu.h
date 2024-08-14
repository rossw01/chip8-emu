#ifndef CPU_H
#define CPU_H

#include "config.h"
#include "cstdint"
#include "display.h"
#include "input.h"


class Cpu {
public:
  Cpu(Config *config, Display *display, Input *input, Memory *memory);
  ~Cpu();

  void Cycle();
  void Reset();

  void OP_00E0(); // CLS
  void OP_00EE(); // RET
  void OP_1nnn(); // JP addr
  void OP_2nnn(); // Call addr
  void OP_3xkk(); // SE Vx, byte
  void OP_4xkk(); // SNE Vx, byte
  void OP_5xy0(); // SE Vx, Vy
  void OP_6xkk(); // LD Vx, byte
  void OP_7xkk(); // ADD Vx, byte
  void OP_8xy0(); // LD Vx, Vy
  void OP_8xy1(); // OR Vx, Vy
  void OP_8xy2(); // AND Vx, Vy
  void OP_8xy3(); // XOR Vx, Vy
  void OP_8xy4(); // ADD Vx, Vy
  void OP_8xy5(); // SUB Vx, Vy
  void OP_8xy6(); // SHR Vx {, Vy}
  void OP_8xy7(); // SUBN Vx, Vy
  void OP_8xyE(); // SHL Vx {, Vy} 
  void OP_9xy0(); // SNE Vx, Vy
  void OP_Annn(); // LD I, addr
  void OP_Bnnn(); // JP V0, addr
  void OP_Cxkk(); // RND Vx, byte
  void OP_Dxyn(); // DRW Vx, Vy, nibble
  void OP_Ex9E(); // SKP Vx
  void OP_ExA1(); // SKNP Vx

  void OP_Fx07(); // LD Vx, DT
  void OP_Fx0A(); // LD Vx, K
  void OP_Fx15(); // LD DT, Vx
  void OP_Fx18(); // LD ST, Vx
  void OP_Fx1E(); // ADD I, Vx
  void OP_Fx29(); // LD F, Vx
  void OP_Fx33(); // LD B, Vx
  void OP_Fx55(); // LD [I], Vx
  void OP_Fx65(); // LD Vx, [I]

  // Function pointer subtables
  void Table0();
  void Table8();
  void TableE();
  void TableF();
  void Void();

private:

  using Instruction = void (Cpu::*)();

  Instruction _table[0xF + 1];
  Instruction _table0[0xE + 1]; // Subtable for opcodes starting with 0x0
  Instruction _table8[0xE + 1]; // Subtable for opcodes starting with 0x8
  Instruction _tableE[0xE + 1]; // Subtable for opcodes starting with 0xE
  Instruction _tableF[0x65 + 1]; // Subtable for opcodes starting with 0xF

  uint16_t _opcode;
  uint8_t _registers[16]{0};
  uint16_t _index{}; // 16bit index register
  uint16_t _programCounter{}; // 16bit program counter
  uint16_t _callStack[16]{0}; // 16 depth, 16bit call stack
  uint8_t _stackPointer{}; // 8bit stack pointer
  uint8_t _delayTimer{}; // If not 0, decrements at 60hz
  uint8_t _soundTimer{}; // Same as delay, buzzer beeps when 0

  Display *_display;
  Config *_config;
  Input *_input;
  Memory *_memory;
};

#endif // !CPU_H
