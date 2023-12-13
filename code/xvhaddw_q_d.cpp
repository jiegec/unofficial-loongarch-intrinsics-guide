#include "common.h"

v256 xvhaddw_q_d(v256 a, v256 b) {
  v256 dst;
#include "xvhaddw_q_d.h"
  return dst;
}

void test() { XFUZZ2(xvhaddw_q_d); }
