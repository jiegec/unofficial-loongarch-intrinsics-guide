#include "common.h"

v128 vdiv_du(v128 a, v128 b) {
  v128 dst;
#include "vdiv_du.h"
  return dst;
}

void test() { FUZZ2(vdiv_du); }
