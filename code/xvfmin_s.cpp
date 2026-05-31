#include "common.h"

v256 xvfmin_s(v256 a, v256 b) {
  v256 dst;
#include "xvfmin_s.h"
  return dst;
}

void test() { XFUZZ2(xvfmin_s); }
