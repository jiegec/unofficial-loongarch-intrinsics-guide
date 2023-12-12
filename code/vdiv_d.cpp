#include "common.h"

v128 vdiv_d(v128 a, v128 b) {
  v128 dst;
#include "vdiv_d.h"
  return dst;
}

void test() { FUZZ2(vdiv_d); }
