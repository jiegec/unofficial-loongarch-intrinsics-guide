#include "common.h"

v256 xvinsve0_w(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvinsve0_w.h"
  return dst;
}

void test() {
  XFUZZ2(xvinsve0_w, 0);
  XFUZZ2(xvinsve0_w, 1);
  XFUZZ2(xvinsve0_w, 2);
}
