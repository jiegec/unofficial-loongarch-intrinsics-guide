#include "common.h"

v256 xvhaddw_du_wu(v256 a, v256 b) {
  v256 dst;
#include "xvhaddw_du_wu.h"
  return dst;
}

void test() { XFUZZ2(xvhaddw_du_wu); }
