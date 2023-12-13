#include "common.h"

v256 xvmax_w(v256 a, v256 b) {
  v256 dst;
#include "xvmax_w.h"
  return dst;
}

void test() { XFUZZ2(xvmax_w); }
