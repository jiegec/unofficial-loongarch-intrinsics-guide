#include "common.h"

v256 xvmul_w(v256 a, v256 b) {
  v256 dst;
#include "xvmul_w.h"
  return dst;
}

void test() { XFUZZ2(xvmul_w); }
