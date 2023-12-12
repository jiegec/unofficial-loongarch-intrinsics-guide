#include "common.h"

v128 vhaddw_hu_bu(v128 a, v128 b) {
  v128 dst;
#include "vhaddw_hu_bu.h"
  return dst;
}

void test() { FUZZ2(vhaddw_hu_bu); }
