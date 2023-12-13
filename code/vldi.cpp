#include "common.h"

v128 vldi(int imm) {
  v128 dst;
#include "vldi.h"
  return dst;
}

void test() {
  // 0b000
  FUZZ0(vldi, (0 << 10) | 0x12);
  // 0b001
  FUZZ0(vldi, (1 << 10) | 0x12);
  // 0b010
  FUZZ0(vldi, (2 << 10) | 0x12);
  // 0b011
  FUZZ0(vldi, (3 << 10) | 0x12);
  // 0b10000
  FUZZ0(vldi, -(((0b01111) << 8) | 0x12));
  // 0b10001
  FUZZ0(vldi, -(((0b01110) << 8) | 0x12));
  // 0b10010
  FUZZ0(vldi, -(((0b01101) << 8) | 0x12));
  // 0b10011
  FUZZ0(vldi, -(((0b01100) << 8) | 0x12));
  // 0b10100
  FUZZ0(vldi, -(((0b01011) << 8) | 0x12));
  // 0b10101
  FUZZ0(vldi, -(((0b01010) << 8) | 0x12));
  // 0b10110
  FUZZ0(vldi, -(((0b01001) << 8) | 0x12));
  // 0b10111
  FUZZ0(vldi, -(((0b01000) << 8) | 0x12));
  // 0b11000
  FUZZ0(vldi, -(((0b00111) << 8) | 0x12));
  // 0b11001
  FUZZ0(vldi, -(((0b00110) << 8) | 0x12));
  // 0b11010
  FUZZ0(vldi, -(((0b00101) << 8) | 0x12));
  // 0b11011
  FUZZ0(vldi, -(((0b00100) << 8) | 0x12));
  // 0b11100
  FUZZ0(vldi, -(((0b00011) << 8) | 0x12));
}
