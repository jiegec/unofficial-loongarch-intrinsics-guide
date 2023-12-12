#include "common.h"

v128 vbitset_h(v128 a, v128 b) {
  v128 dst;
#include "vbitset_h.h"
  return dst;
}

void test() { FUZZ2(vbitset_h); }
