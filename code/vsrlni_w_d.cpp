#include "common.h"

v128 vsrlni_w_d(v128 a, v128 b, int imm) {
  v128 dst;
#include "vsrlni_w_d.h"
  return dst;
}

void test() {
  FUZZ2(vsrlni_w_d, 0);
  FUZZ2(vsrlni_w_d, 7);
  FUZZ2(vsrlni_w_d, 15);
}
