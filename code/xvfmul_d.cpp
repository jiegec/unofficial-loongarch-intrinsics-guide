#include "common.h"

v256 xvfmul_d(v256 a, v256 b) {
  v256 dst;
#include "xvfmul_d.h"
  return dst;
}

void test() { XFUZZ2(xvfmul_d); }
