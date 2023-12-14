#include "common.h"

v256 xvandn_v(v256 a, v256 b) {
  v256 dst;
#include "xvandn_v.h"
  return dst;
}

void test() { XFUZZ2(xvandn_v); }
