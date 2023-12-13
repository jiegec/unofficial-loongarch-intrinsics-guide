#include "common.h"

v256 xvmuh_b(v256 a, v256 b) {
  v256 dst;
#include "xvmuh_b.h"
  return dst;
}

void test() { XFUZZ2(xvmuh_b); }
