#include "common.h"

v128 vssub_b(v128 a, v128 b) {
  v128 dst;
#include "vssub_b.h"
  return dst;
}

void test() { FUZZ2(vssub_b); }
