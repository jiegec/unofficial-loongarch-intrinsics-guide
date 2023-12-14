#include "common.h"

v256 xvnor_v(v256 a, v256 b) {
  v256 dst;
#include "xvnor_v.h"
  return dst;
}

void test() { XFUZZ2(xvnor_v); }
