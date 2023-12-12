#include "common.h"

v128 vslt_hu(v128 a, v128 b) {
  v128 dst;
#include "vslt_hu.h"
  return dst;
}

void test() { FUZZ2(vslt_hu); }
