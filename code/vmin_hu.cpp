#include "common.h"

v128 vmin_hu(v128 a, v128 b) {
  v128 dst;
#include "vmin_hu.h"
  return dst;
}

void test() { FUZZ2(vmin_hu); }
