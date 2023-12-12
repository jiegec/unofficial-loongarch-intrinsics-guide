#include "common.h"

v128 vhaddw_wu_hu(v128 a, v128 b) {
  v128 dst;
#include "vhaddw_wu_hu.h"
  return dst;
}

void test() { FUZZ2(vhaddw_wu_hu); }
