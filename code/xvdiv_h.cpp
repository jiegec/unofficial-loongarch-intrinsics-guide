#include "common.h"

v256 xvdiv_h(v256 a, v256 b) {
  v256 dst;
#include "xvdiv_h.h"
  return dst;
}

void test() { XFUZZ2(xvdiv_h); }
