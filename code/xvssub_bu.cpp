#include "common.h"

v256 xvssub_bu(v256 a, v256 b) {
  v256 dst;
#include "xvssub_bu.h"
  return dst;
}

void test() { XFUZZ2(xvssub_bu); }
