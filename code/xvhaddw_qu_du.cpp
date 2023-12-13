#include "common.h"

v256 xvhaddw_qu_du(v256 a, v256 b) {
  v256 dst;
#include "xvhaddw_qu_du.h"
  return dst;
}

void test() { XFUZZ2(xvhaddw_qu_du); }
