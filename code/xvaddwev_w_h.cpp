#include "common.h"

v256 xvaddwev_w_h(v256 a, v256 b) {
  v256 dst;
#include "xvaddwev_w_h.h"
  return dst;
}

void test() { XFUZZ2(xvaddwev_w_h); }
