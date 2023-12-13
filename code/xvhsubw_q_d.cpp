#include "common.h"

v256 xvhsubw_q_d(v256 a, v256 b) {
  v256 dst;
#include "xvhsubw_q_d.h"
  return dst;
}

void test() { XFUZZ2(xvhsubw_q_d); }
