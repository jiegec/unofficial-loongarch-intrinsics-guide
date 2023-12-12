#include "common.h"

v128 vhsubw_qu_du(v128 a, v128 b) {
  v128 dst;
#include "vhsubw_qu_du.h"
  return dst;
}

void test() { FUZZ2(vhsubw_qu_du); }
