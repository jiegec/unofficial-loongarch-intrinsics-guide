#include "common.h"

v256 xvxor_v(v256 a, v256 b) {
  v256 dst;
#include "xvxor_v.h"
  return dst;
}

void test() { XFUZZ2(xvxor_v); }
