#include "common.h"

v128 vadd_b(v128 a, v128 b) {
  v128 dst;
#include "vadd_b.h"
  return dst;
}

void test() { FUZZ2(vadd_b); }
