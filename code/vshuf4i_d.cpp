#include "common.h"

v128 vshuf4i_d(v128 a, v128 b, int imm) {
  v128 dst;
#include "vshuf4i_d.h"
  return dst;
}

void test() {
  FUZZ2(vshuf4i_d, 0);
  FUZZ2(vshuf4i_d, 13);
  FUZZ2(vshuf4i_d, 100);
  FUZZ2(vshuf4i_d, 128);
  FUZZ2(vshuf4i_d, 255);
}
