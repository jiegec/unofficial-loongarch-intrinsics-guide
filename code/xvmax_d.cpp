#include "common.h"

v256 xvmax_d(v256 a, v256 b) {
  v256 dst;
#include "xvmax_d.h"
  return dst;
}

void test() { XFUZZ2(xvmax_d); }
