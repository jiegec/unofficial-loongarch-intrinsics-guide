#include "common.h"

v128 vssrarn_h_w(v128 a, v128 b) {
  v128 dst;
#include "vssrarn_h_w.h"
  return dst;
}

void test() { FUZZ2(vssrarn_h_w); }
