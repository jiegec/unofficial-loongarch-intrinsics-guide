#include "common.h"

v128 vbitset_b(v128 a, v128 b) {
  v128 dst;
#include "vbitset_b.h"
  return dst;
}

void test() { FUZZ2(vbitset_b); }
