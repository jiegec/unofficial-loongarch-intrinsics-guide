#include "common.h"

v256 xvmax_hu(v256 a, v256 b) {
  v256 dst;
#include "xvmax_hu.h"
  return dst;
}

void test() { XFUZZ2(xvmax_hu); }
