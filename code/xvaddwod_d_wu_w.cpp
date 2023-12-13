#include "common.h"

v256 xvaddwod_d_wu_w(v256 a, v256 b) {
  v256 dst;
#include "xvaddwod_d_wu_w.h"
  return dst;
}

void test() { XFUZZ2(xvaddwod_d_wu_w); }
