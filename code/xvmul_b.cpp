#include "common.h"

v256 xvmul_b(v256 a, v256 b) {
  v256 dst;
#include "xvmul_b.h"
  return dst;
}

void test() { XFUZZ2(xvmul_b); }
