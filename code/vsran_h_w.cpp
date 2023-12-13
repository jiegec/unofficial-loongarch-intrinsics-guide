#include "common.h"

v128 vsran_h_w(v128 a, v128 b) {
  v128 dst;
#include "vsran_h_w.h"
  return dst;
}

void test() { FUZZ2(vsran_h_w); }
