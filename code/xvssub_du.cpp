#include "common.h"

v256 xvssub_du(v256 a, v256 b) {
  v256 dst;
#include "xvssub_du.h"
  return dst;
}

void test() { XFUZZ2(xvssub_du); }
