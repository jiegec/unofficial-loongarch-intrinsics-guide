#include "common.h"

v128 vfmin_s(v128 a, v128 b) {
  v128 dst;
#include "vfmin_s.h"
  return dst;
}

void test() { FUZZ2(vfmin_s); }
