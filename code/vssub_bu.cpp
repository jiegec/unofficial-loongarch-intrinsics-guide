#include "common.h"

v128 vssub_bu(v128 a, v128 b) {
  v128 dst;
#include "vssub_bu.h"
  return dst;
}

void test() { FUZZ2(vssub_bu); }
