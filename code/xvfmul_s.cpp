#include "common.h"

v256 xvfmul_s(v256 a, v256 b) {
  v256 dst;
#include "xvfmul_s.h"
  return dst;
}

void test() { XFUZZ2(xvfmul_s); }
