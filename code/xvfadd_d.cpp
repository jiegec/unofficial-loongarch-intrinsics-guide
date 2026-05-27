#include "common.h"

v256 xvfadd_d(v256 a, v256 b) {
  v256 dst;
#include "xvfadd_d.h"
  return dst;
}

void test() { XFUZZ2(xvfadd_d); }
