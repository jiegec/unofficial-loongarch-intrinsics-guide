#include "common.h"

v128 vslt_w(v128 a, v128 b) {
  v128 dst;
#include "vslt_w.h"
  return dst;
}

void test() { FUZZ2(vslt_w); }
