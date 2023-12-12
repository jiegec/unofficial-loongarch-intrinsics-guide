#include "common.h"

v128 vmin_d(v128 a, v128 b) {
  v128 dst;
#include "vmin_d.h"
  return dst;
}

void test() { FUZZ2(vmin_d); }
