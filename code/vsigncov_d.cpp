#include "common.h"

v128 vsigncov_d(v128 a, v128 b) {
  v128 dst;
#include "vsigncov_d.h"
  return dst;
}

void test() { FUZZ2(vsigncov_d); }
