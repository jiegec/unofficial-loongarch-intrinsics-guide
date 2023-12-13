#include "common.h"

v256 xvdiv_d(v256 a, v256 b) {
  v256 dst;
#include "xvdiv_d.h"
  return dst;
}

void test() { XFUZZ2(xvdiv_d); }
