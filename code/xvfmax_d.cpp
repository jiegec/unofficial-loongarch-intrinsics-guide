#include "common.h"

v256 xvfmax_d(v256 a, v256 b) {
  v256 dst;
#include "xvfmax_d.h"
  return dst;
}

void test() { XFUZZ2(xvfmax_d); }
