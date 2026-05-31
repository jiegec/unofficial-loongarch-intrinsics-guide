#include "common.h"

v256 xvfmin_d(v256 a, v256 b) {
  v256 dst;
#include "xvfmin_d.h"
  return dst;
}

void test() { XFUZZ2(xvfmin_d); }
