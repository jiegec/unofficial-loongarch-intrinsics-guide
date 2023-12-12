#include "common.h"

v128 vsra_w(v128 a, v128 b) {
  v128 dst;
#include "vsra_w.h"
  return dst;
}

void test() { FUZZ2(vsra_w); }
