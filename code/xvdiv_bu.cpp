#include "common.h"

v256 xvdiv_bu(v256 a, v256 b) {
  v256 dst;
#include "xvdiv_bu.h"
  return dst;
}

void test() { XFUZZ2(xvdiv_bu); }
