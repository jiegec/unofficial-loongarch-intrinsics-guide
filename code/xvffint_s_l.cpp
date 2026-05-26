#include "common.h"

v256 xvffint_s_l(v256 a, v256 b) {
  v256 dst;
#include "xvffint_s_l.h"
  return dst;
}

void test() { XFUZZ2(xvffint_s_l); }
