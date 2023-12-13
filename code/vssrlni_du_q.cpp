#include "common.h"

v128 vssrlni_du_q(v128 a, v128 b, int imm) {
  v128 dst;
#include "vssrlni_du_q.h"
  return dst;
}

void test() {
  FUZZ2(vssrlni_du_q, 0);
  FUZZ2(vssrlni_du_q, 7);
  FUZZ2(vssrlni_du_q, 15);
}
