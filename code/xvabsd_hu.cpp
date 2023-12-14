#include "common.h"

v256 xvabsd_hu(v256 a, v256 b) {
  v256 dst;
#include "xvabsd_hu.h"
  return dst;
}

void test() { XFUZZ2(xvabsd_hu); }
