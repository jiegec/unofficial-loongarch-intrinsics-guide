#include "common.h"

v128 vssub_hu(v128 a, v128 b) {
  v128 dst;
#include "vssub_hu.h"
  return dst;
}

void test() { FUZZ2(vssub_hu); }
