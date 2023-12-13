#include "common.h"

v128 vsrani_d_q(v128 a, v128 b, int imm) {
  v128 dst;
#include "vsrani_d_q.h"
  return dst;
}

void test() {
  FUZZ2(vsrani_d_q, 0);
  FUZZ2(vsrani_d_q, 7);
}
