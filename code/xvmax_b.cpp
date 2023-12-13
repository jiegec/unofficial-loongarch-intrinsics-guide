#include "common.h"

v256 xvmax_b(v256 a, v256 b) {
  v256 dst;
#include "xvmax_b.h"
  return dst;
}

void test() { XFUZZ2(xvmax_b); }
