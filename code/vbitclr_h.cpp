#include "common.h"

v128 vbitclr_h(v128 a, v128 b) {
  v128 dst;
#include "vbitclr_h.h"
  return dst;
}

void test() { FUZZ2(vbitclr_h); }
