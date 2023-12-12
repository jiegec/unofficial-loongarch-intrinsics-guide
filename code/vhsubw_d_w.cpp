#include "common.h"

v128 vhsubw_d_w(v128 a, v128 b) {
  v128 dst;
#include "vhsubw_d_w.h"
  return dst;
}

void test() { FUZZ2(vhsubw_d_w); }
