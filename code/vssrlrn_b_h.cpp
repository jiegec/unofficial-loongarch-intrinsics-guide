#include "common.h"

v128 vssrlrn_b_h(v128 a, v128 b) {
  v128 dst;
#include "vssrlrn_b_h.h"
  return dst;
}

void test() { FUZZ2(vssrlrn_b_h); }
