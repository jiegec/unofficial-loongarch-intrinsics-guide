#include "common.h"

v128 vdiv_b(v128 a, v128 b) {
  v128 dst;
#include "vdiv_b.h"
  return dst;
}

void test() { FUZZ2(vdiv_b); }
