#include "common.h"

v128 vsigncov_w(v128 a, v128 b) {
  v128 dst;
#include "vsigncov_w.h"
  return dst;
}

void test() { FUZZ2(vsigncov_w); }
