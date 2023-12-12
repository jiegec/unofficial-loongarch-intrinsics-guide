#include "common.h"

v128 vsrlr_d(v128 a, v128 b) {
  v128 dst;
#include "vsrlr_d.h"
  return dst;
}

void test() { FUZZ2(vsrlr_d); }
