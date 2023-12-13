#include "common.h"

v128 vssrlni_b_h(v128 a, v128 b, int imm) {
  v128 dst;
#include "vssrlni_b_h.h"
  return dst;
}

void test() {
  FUZZ2(vssrlni_b_h, 0);
  FUZZ2(vssrlni_b_h, 7);
  FUZZ2(vssrlni_b_h, 15);
}
