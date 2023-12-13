#include "common.h"

v256 xvmax_bu(v256 a, v256 b) {
  v256 dst;
#include "xvmax_bu.h"
  return dst;
}

void test() { XFUZZ2(xvmax_bu); }
