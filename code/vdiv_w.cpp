#include "common.h"

v128 vdiv_w(v128 a, v128 b) {
  v128 dst;
#include "vdiv_w.h"
  return dst;
}

void test() { FUZZ2(vdiv_w); }
