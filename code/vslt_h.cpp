#include "common.h"

v128 vslt_h(v128 a, v128 b) {
  v128 dst;
#include "vslt_h.h"
  return dst;
}

void test() { FUZZ2(vslt_h); }
