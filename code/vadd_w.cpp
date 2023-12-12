#include "common.h"

v128 vadd_w(v128 a, v128 b) {
  v128 dst;
#include "vadd_w.h"
  return dst;
}

void test() { FUZZ2(vadd_w); }
