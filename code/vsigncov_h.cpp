#include "common.h"

v128 vsigncov_h(v128 a, v128 b) {
  v128 dst;
#include "vsigncov_h.h"
  return dst;
}

void test() { FUZZ2(vsigncov_h); }
