#include "common.h"

v128 vadd_d(v128 a, v128 b) {
  v128 dst;
#include "vadd_d.h"
  return dst;
}

void test() { FUZZ2(vadd_d); }
