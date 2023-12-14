#include "common.h"

v256 xvbitseli_b(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvbitseli_b.h"
  return dst;
}

void test() {
  XFUZZ2(xvbitseli_b, 0x00);
  XFUZZ2(xvbitseli_b, 0x01);
  XFUZZ2(xvbitseli_b, 0x01);
  XFUZZ2(xvbitseli_b, 0x80);
  XFUZZ2(xvbitseli_b, 0xFF);
}
