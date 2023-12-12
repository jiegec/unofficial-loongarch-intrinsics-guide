#include "common.h"

v128 vhaddw_qu_du(v128 a, v128 b) {
  v128 dst;
#include "vhaddw_qu_du.h"
  return dst;
}

void test() { FUZZ2(vhaddw_qu_du); }
