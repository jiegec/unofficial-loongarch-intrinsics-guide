#include "common.h"

v128 vilvl_w(v128 a, v128 b) {
  v128 dst;
#include "vilvl_w.h"
  return dst;
}

void test() { FUZZ2(vilvl_w); }
