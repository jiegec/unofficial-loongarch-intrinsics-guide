#include "common.h"

v256 xvfcvt_s_d(v256 a, v256 b) {
  v256 dst;
#include "xvfcvt_s_d.h"
  return dst;
}

void test() { XFUZZ2(xvfcvt_s_d); }
