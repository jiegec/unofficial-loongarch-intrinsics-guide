#include "common.h"

v256 xvldi(int imm) {
  v256 dst;
#include "xvldi.h"
  return dst;
}

void test() {
  // 0b000
  XFUZZ0(xvldi, (0 << 10) | 0x12);
  // 0b001
  XFUZZ0(xvldi, (1 << 10) | 0x12);
  // 0b010
  XFUZZ0(xvldi, (2 << 10) | 0x12);
  // 0b011
  XFUZZ0(xvldi, (3 << 10) | 0x12);
  // 0b10000
  XFUZZ0(xvldi, -(((0b01111) << 8) | 0x12));
  // 0b10001
  XFUZZ0(xvldi, -(((0b01110) << 8) | 0x12));
  // 0b10010
  XFUZZ0(xvldi, -(((0b01101) << 8) | 0x12));
  // 0b10011
  XFUZZ0(xvldi, -(((0b01100) << 8) | 0x12));
  // 0b10100
  XFUZZ0(xvldi, -(((0b01011) << 8) | 0x12));
  // 0b10101
  XFUZZ0(xvldi, -(((0b01010) << 8) | 0x12));
  // 0b10110
  XFUZZ0(xvldi, -(((0b01001) << 8) | 0x12));
  // 0b10111
  XFUZZ0(xvldi, -(((0b01000) << 8) | 0x12));
  // 0b11000
  XFUZZ0(xvldi, -(((0b00111) << 8) | 0x12));
  // 0b11001
  XFUZZ0(xvldi, -(((0b00110) << 8) | 0x12));
  // 0b11010
  XFUZZ0(xvldi, -(((0b00101) << 8) | 0x12));
  // 0b11011
  XFUZZ0(xvldi, -(((0b00100) << 8) | 0x12));
  // 0b11100
  XFUZZ0(xvldi, -(((0b00011) << 8) | 0x12));
}
