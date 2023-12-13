#include "common.h"

v128 vssrlrn_w_d(v128 a, v128 b) {
  v128 dst;
#include "vssrlrn_w_d.h"
  return dst;
}

void test() { FUZZ2(vssrlrn_w_d); }
