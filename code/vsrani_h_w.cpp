#include "common.h"

v128 vsrani_h_w(v128 a, v128 b, int imm) {
  v128 dst;
#include "vsrani_h_w.h"
  return dst;
}

void test() {
  FUZZ2(vsrani_h_w, 0);
  FUZZ2(vsrani_h_w, 7);
}
