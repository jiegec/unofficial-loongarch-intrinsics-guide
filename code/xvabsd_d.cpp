#include "common.h"

v256 xvabsd_d(v256 a, v256 b) {
  v256 dst;
#include "xvabsd_d.h"
  return dst;
}

void test() { XFUZZ2(xvabsd_d); }
