#include "common.h"

v256 xvaddwod_w_hu(v256 a, v256 b) {
  v256 dst;
#include "xvaddwod_w_hu.h"
  return dst;
}

void test() { XFUZZ2(xvaddwod_w_hu); }
