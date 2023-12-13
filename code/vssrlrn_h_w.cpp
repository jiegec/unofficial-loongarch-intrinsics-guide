#include "common.h"

v128 vssrlrn_h_w(v128 a, v128 b) {
  v128 dst;
#include "vssrlrn_h_w.h"
  return dst;
}

void test() { FUZZ2(vssrlrn_h_w); }
