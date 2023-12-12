#include "common.h"

v128 vmin_b(v128 a, v128 b) {
  v128 dst;
#include "vmin_b.h"
  return dst;
}

void test() { FUZZ2(vmin_b); }
