#include "common.h"

v256 xvbitsel_v(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvbitsel_v.h"
  return dst;
}

void test() { XFUZZ3(xvbitsel_v); }
