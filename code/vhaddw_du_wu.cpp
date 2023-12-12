#include "common.h"

v128 vhaddw_du_wu(v128 a, v128 b) {
  v128 dst;
#include "vhaddw_du_wu.h"
  return dst;
}

void test() { FUZZ2(vhaddw_du_wu); }
