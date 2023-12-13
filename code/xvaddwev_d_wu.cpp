#include "common.h"

v256 xvaddwev_d_wu(v256 a, v256 b) {
  v256 dst;
#include "xvaddwev_d_wu.h"
  return dst;
}

void test() { XFUZZ2(xvaddwev_d_wu); }
