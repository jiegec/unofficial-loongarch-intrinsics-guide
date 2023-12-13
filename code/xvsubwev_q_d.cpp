#include "common.h"

v256 xvsubwev_q_d(v256 a, v256 b) {
  v256 dst;
#include "xvsubwev_q_d.h"
  return dst;
}

void test() { XFUZZ2(xvsubwev_q_d); }
