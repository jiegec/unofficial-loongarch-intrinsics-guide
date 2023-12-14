#include "common.h"

v256 xvinsve0_d(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvinsve0_d.h"
  return dst;
}

void test() {
  XFUZZ2(xvinsve0_d, 0);
  XFUZZ2(xvinsve0_d, 1);
  XFUZZ2(xvinsve0_d, 2);
}
