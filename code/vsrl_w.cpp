#include "common.h"

v128 vsrl_w(v128 a, v128 b) {
  v128 dst;
#include "vsrl_w.h"
  return dst;
}

void test() { FUZZ2(vsrl_w); }
