#include "common.h"

v256 xvabsd_wu(v256 a, v256 b) {
  v256 dst;
#include "xvabsd_wu.h"
  return dst;
}

void test() { XFUZZ2(xvabsd_wu); }
