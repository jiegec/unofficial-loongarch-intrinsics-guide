#include "common.h"

v256 xvdiv_wu(v256 a, v256 b) {
  v256 dst;
#include "xvdiv_wu.h"
  return dst;
}

void test() { XFUZZ2(xvdiv_wu); }
