#include "common.h"

v256 xvaddwev_h_bu(v256 a, v256 b) {
  v256 dst;
#include "xvaddwev_h_bu.h"
  return dst;
}

void test() { XFUZZ2(xvaddwev_h_bu); }
