#include "common.h"

v128 vssrlrn_hu_w(v128 a, v128 b) {
  v128 dst;
#include "vssrlrn_hu_w.h"
  return dst;
}

void test() { FUZZ2(vssrlrn_hu_w); }
