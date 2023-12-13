#include "common.h"

v256 xvaddwev_q_du(v256 a, v256 b) {
  v256 dst;
#include "xvaddwev_q_du.h"
  return dst;
}

void test() { XFUZZ2(xvaddwev_q_du); }
