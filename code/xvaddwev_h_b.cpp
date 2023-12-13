#include "common.h"

v256 xvaddwev_h_b(v256 a, v256 b) {
  v256 dst;
#include "xvaddwev_h_b.h"
  return dst;
}

void test() { XFUZZ2(xvaddwev_h_b); }
