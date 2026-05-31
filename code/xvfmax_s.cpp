#include "common.h"

v256 xvfmax_s(v256 a, v256 b) {
  v256 dst;
#include "xvfmax_s.h"
  return dst;
}

void test() { XFUZZ2(xvfmax_s); }
