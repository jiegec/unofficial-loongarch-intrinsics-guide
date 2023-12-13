#include "common.h"

v256 xvhaddw_d_w(v256 a, v256 b) {
  v256 dst;
#include "xvhaddw_d_w.h"
  return dst;
}

void test() { XFUZZ2(xvhaddw_d_w); }
