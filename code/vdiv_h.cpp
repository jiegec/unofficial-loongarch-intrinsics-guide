#include "common.h"

v128 vdiv_h(v128 a, v128 b) {
  v128 dst;
#include "vdiv_h.h"
  return dst;
}

void test() { FUZZ2(vdiv_h); }
