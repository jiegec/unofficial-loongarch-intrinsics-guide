#include "common.h"

v128 vslt_b(v128 a, v128 b) {
  v128 dst;
#include "vslt_b.h"
  return dst;
}

void test() { FUZZ2(vslt_b); }
