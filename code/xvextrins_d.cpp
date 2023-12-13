#include "common.h"

v256 xvextrins_d(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvextrins_d.h"
  return dst;
}

void test() {
  XFUZZ2(xvextrins_d, 0);
  XFUZZ2(xvextrins_d, 3);
  XFUZZ2(xvextrins_d, 7);
  XFUZZ2(xvextrins_d, 15);
  XFUZZ2(xvextrins_d, 16);
  XFUZZ2(xvextrins_d, 32);
  XFUZZ2(xvextrins_d, 64);
  XFUZZ2(xvextrins_d, 128);
  XFUZZ2(xvextrins_d, 255);
}
