#include "common.h"

v256 xvdiv_b(v256 a, v256 b) {
  v256 dst;
#include "xvdiv_b.h"
  return dst;
}

void test() { XFUZZ2(xvdiv_b); }
