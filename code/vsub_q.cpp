#include "common.h"

v128 vsub_q(v128 a, v128 b) {
  v128 dst;
#include "vsub_q.h"
  return dst;
}

void test() { FUZZ2(vsub_q); }
