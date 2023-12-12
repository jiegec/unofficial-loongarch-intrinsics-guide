#include "common.h"

v128 vmin_w(v128 a, v128 b) {
  v128 dst;
#include "vmin_w.h"
  return dst;
}

void test() { FUZZ2(vmin_w); }
