#include "common.h"

v128 vslt_bu(v128 a, v128 b) {
  v128 dst;
#include "vslt_bu.h"
  return dst;
}

void test() { FUZZ2(vslt_bu); }
