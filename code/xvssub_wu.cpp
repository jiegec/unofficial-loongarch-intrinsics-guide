#include "common.h"

v256 xvssub_wu(v256 a, v256 b) {
  v256 dst;
#include "xvssub_wu.h"
  return dst;
}

void test() { XFUZZ2(xvssub_wu); }
