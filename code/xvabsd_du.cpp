#include "common.h"

v256 xvabsd_du(v256 a, v256 b) {
  v256 dst;
#include "xvabsd_du.h"
  return dst;
}

void test() { XFUZZ2(xvabsd_du); }
