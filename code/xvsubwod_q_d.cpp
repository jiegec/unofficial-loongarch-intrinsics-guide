#include "common.h"

v256 xvsubwod_q_d(v256 a, v256 b) {
  v256 dst;
#include "xvsubwod_q_d.h"
  return dst;
}

void test() { XFUZZ2(xvsubwod_q_d); }
