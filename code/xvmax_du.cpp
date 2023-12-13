#include "common.h"

v256 xvmax_du(v256 a, v256 b) {
  v256 dst;
#include "xvmax_du.h"
  return dst;
}

void test() { XFUZZ2(xvmax_du); }
