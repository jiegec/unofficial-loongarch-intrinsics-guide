#include "common.h"

v256 xvsran_h_w(v256 a, v256 b) {
  v256 dst;
#include "xvsran_h_w.h"
  return dst;
}

void test() { XFUZZ2(xvsran_h_w); }
