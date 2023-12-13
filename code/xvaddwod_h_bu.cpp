#include "common.h"

v256 xvaddwod_h_bu(v256 a, v256 b) {
  v256 dst;
#include "xvaddwod_h_bu.h"
  return dst;
}

void test() { XFUZZ2(xvaddwod_h_bu); }
