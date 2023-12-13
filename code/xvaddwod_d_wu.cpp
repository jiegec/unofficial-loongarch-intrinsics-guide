#include "common.h"

v256 xvaddwod_d_wu(v256 a, v256 b) {
  v256 dst;
#include "xvaddwod_d_wu.h"
  return dst;
}

void test() { XFUZZ2(xvaddwod_d_wu); }
