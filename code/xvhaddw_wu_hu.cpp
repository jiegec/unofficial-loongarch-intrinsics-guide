#include "common.h"

v256 xvhaddw_wu_hu(v256 a, v256 b) {
  v256 dst;
#include "xvhaddw_wu_hu.h"
  return dst;
}

void test() { XFUZZ2(xvhaddw_wu_hu); }
