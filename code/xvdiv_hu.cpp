#include "common.h"

v256 xvdiv_hu(v256 a, v256 b) {
  v256 dst;
#include "xvdiv_hu.h"
  return dst;
}

void test() { XFUZZ2(xvdiv_hu); }
