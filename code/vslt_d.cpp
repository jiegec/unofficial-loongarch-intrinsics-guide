#include "common.h"

v128 vslt_d(v128 a, v128 b) {
  v128 dst;
#include "vslt_d.h"
  return dst;
}

void test() { FUZZ2(vslt_d); }
