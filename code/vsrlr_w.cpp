#include "common.h"

v128 vsrlr_w(v128 a, v128 b) {
  v128 dst;
#include "vsrlr_w.h"
  return dst;
}

void test() { FUZZ2(vsrlr_w); }
