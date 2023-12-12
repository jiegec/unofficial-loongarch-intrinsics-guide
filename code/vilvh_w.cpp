#include "common.h"

v128 vilvh_w(v128 a, v128 b) {
  v128 dst;
#include "vilvh_w.h"
  return dst;
}

void test() { FUZZ2(vilvh_w); }
