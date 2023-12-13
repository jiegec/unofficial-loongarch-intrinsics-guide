#include "common.h"

v256 xvdiv_w(v256 a, v256 b) {
  v256 dst;
#include "xvdiv_w.h"
  return dst;
}

void test() { XFUZZ2(xvdiv_w); }
