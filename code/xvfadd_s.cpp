#include "common.h"

v256 xvfadd_s(v256 a, v256 b) {
  v256 dst;
#include "xvfadd_s.h"
  return dst;
}

void test() { XFUZZ2(xvfadd_s); }
