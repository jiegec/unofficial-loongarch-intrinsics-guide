#include "common.h"

v256 xvmaddwod_d_w(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmaddwod_d_w.h"
  return dst;
}

void test() { XFUZZ3(xvmaddwod_d_w); }
