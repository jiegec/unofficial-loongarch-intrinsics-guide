#include "common.h"

v256 xvmaddwod_h_b(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmaddwod_h_b.h"
  return dst;
}

void test() { XFUZZ3(xvmaddwod_h_b); }
