#include "common.h"

v256 xvdiv_du(v256 a, v256 b) {
  v256 dst;
#include "xvdiv_du.h"
  return dst;
}

void test() { XFUZZ2(xvdiv_du); }
