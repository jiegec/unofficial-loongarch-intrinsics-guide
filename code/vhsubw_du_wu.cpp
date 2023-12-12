#include "common.h"

v128 vhsubw_du_wu(v128 a, v128 b) {
  v128 dst;
#include "vhsubw_du_wu.h"
  return dst;
}

void test() { FUZZ2(vhsubw_du_wu); }
