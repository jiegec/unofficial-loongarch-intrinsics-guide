#include "common.h"

v128 vbitclr_d(v128 a, v128 b) {
  v128 dst;
#include "vbitclr_d.h"
  return dst;
}

void test() { FUZZ2(vbitclr_d); }
