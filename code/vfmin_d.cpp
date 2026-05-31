#include "common.h"

v128 vfmin_d(v128 a, v128 b) {
  v128 dst;
#include "vfmin_d.h"
  return dst;
}

void test() { FUZZ2(vfmin_d); }
