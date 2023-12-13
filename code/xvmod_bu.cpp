#include "common.h"

v256 xvmod_bu(v256 a, v256 b) {
  v256 dst;
#include "xvmod_bu.h"
  return dst;
}

void test() { XFUZZ2(xvmod_bu); }
