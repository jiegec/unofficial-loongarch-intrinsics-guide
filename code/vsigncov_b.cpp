#include "common.h"

v128 vsigncov_b(v128 a, v128 b) {
  v128 dst;
#include "vsigncov_b.h"
  return dst;
}

void test() { FUZZ2(vsigncov_b); }
