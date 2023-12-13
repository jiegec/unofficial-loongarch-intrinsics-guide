#include "common.h"

v256 xvmul_h(v256 a, v256 b) {
  v256 dst;
#include "xvmul_h.h"
  return dst;
}

void test() { XFUZZ2(xvmul_h); }
