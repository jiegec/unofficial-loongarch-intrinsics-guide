#include "common.h"

v256 xvsadd_du(v256 a, v256 b) {
  v256 dst;
#include "xvsadd_du.h"
  return dst;
}

void test() { XFUZZ2(xvsadd_du); }
