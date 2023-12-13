#include "common.h"

v128 vssrarn_w_d(v128 a, v128 b) {
  v128 dst;
#include "vssrarn_w_d.h"
  return dst;
}

void test() { FUZZ2(vssrarn_w_d); }
