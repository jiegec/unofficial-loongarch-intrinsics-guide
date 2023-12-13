#include "common.h"

v256 xvmulwev_q_d(v256 a, v256 b) {
  v256 dst;
#include "xvmulwev_q_d.h"
  return dst;
}

void test() { XFUZZ2(xvmulwev_q_d); }
