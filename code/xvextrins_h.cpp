#include "common.h"

v256 xvextrins_h(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvextrins_h.h"
  return dst;
}

void test() {
  XFUZZ2(xvextrins_h, 0);
  XFUZZ2(xvextrins_h, 3);
  XFUZZ2(xvextrins_h, 7);
  XFUZZ2(xvextrins_h, 15);
  XFUZZ2(xvextrins_h, 16);
  XFUZZ2(xvextrins_h, 32);
  XFUZZ2(xvextrins_h, 64);
  XFUZZ2(xvextrins_h, 128);
  XFUZZ2(xvextrins_h, 255);
}
