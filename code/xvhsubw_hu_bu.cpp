#include "common.h"

v256 xvhsubw_hu_bu(v256 a, v256 b) {
  v256 dst;
#include "xvhsubw_hu_bu.h"
  return dst;
}

void test() { XFUZZ2(xvhsubw_hu_bu); }
