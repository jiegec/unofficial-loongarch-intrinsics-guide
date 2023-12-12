#include "common.h"

v128 vsrlr_b(v128 a, v128 b) {
  v128 dst;
#include "vsrlr_b.h"
  return dst;
}

void test() { FUZZ2(vsrlr_b); }
