#include "common.h"

v128 vbitclr_b(v128 a, v128 b) {
  v128 dst;
#include "vbitclr_b.h"
  return dst;
}

void test() { FUZZ2(vbitclr_b); }
