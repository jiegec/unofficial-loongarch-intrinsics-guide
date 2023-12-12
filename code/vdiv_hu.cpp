#include "common.h"

v128 vdiv_hu(v128 a, v128 b) {
  v128 dst;
#include "vdiv_hu.h"
  return dst;
}

void test() { FUZZ2(vdiv_hu); }
