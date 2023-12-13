#include "common.h"

v256 xvmaddwev_h_b(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmaddwev_h_b.h"
  return dst;
}

void test() { XFUZZ3(xvmaddwev_h_b); }
