#include "common.h"

v128 vsrlni_d_q(v128 a, v128 b, int imm) {
  v128 dst;
#include "vsrlni_d_q.h"
  return dst;
}

void test() {
  FUZZ2(vsrlni_d_q, 0);
  FUZZ2(vsrlni_d_q, 7);
  FUZZ2(vsrlni_d_q, 15);
}
