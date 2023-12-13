#include "common.h"

v128 vssrarn_hu_w(v128 a, v128 b) {
  v128 dst;
#include "vssrarn_hu_w.h"
  return dst;
}

void test() { FUZZ2(vssrarn_hu_w); }
