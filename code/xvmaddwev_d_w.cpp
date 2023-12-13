#include "common.h"

v256 xvmaddwev_d_w(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmaddwev_d_w.h"
  return dst;
}

void test() { XFUZZ3(xvmaddwev_d_w); }
