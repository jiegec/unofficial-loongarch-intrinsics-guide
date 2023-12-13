#include "common.h"

v256 xvhsubw_qu_du(v256 a, v256 b) {
  v256 dst;
#include "xvhsubw_qu_du.h"
  return dst;
}

void test() { XFUZZ2(xvhsubw_qu_du); }
