#include "common.h"

v256 xvsran_w_d(v256 a, v256 b) {
  v256 dst;
#include "xvsran_w_d.h"
  return dst;
}

void test() { XFUZZ2(xvsran_w_d); }
