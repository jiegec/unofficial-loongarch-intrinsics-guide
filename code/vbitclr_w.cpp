#include "common.h"

v128 vbitclr_w(v128 a, v128 b) {
  v128 dst;
#include "vbitclr_w.h"
  return dst;
}

void test() { FUZZ2(vbitclr_w); }
