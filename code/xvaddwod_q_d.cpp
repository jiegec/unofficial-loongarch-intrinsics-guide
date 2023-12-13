#include "common.h"

v256 xvaddwod_q_d(v256 a, v256 b) {
  v256 dst;
#include "xvaddwod_q_d.h"
  return dst;
}

void test() { XFUZZ2(xvaddwod_q_d); }
