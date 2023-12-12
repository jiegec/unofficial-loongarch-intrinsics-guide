#include "common.h"

v128 vbitset_d(v128 a, v128 b) {
  v128 dst;
#include "vbitset_d.h"
  return dst;
}

void test() { FUZZ2(vbitset_d); }
