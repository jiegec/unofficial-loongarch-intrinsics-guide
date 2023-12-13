#include "common.h"

v256 xvaddwod_q_du(v256 a, v256 b) {
  v256 dst;
#include "xvaddwod_q_du.h"
  return dst;
}

void test() { XFUZZ2(xvaddwod_q_du); }
