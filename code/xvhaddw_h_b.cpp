#include "common.h"

v256 xvhaddw_h_b(v256 a, v256 b) {
  v256 dst;
#include "xvhaddw_h_b.h"
  return dst;
}

void test() { XFUZZ2(xvhaddw_h_b); }
