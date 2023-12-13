#include "common.h"

v256 xvextrins_b(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvextrins_b.h"
  return dst;
}

void test() {
  XFUZZ2(xvextrins_b, 0);
  XFUZZ2(xvextrins_b, 3);
  XFUZZ2(xvextrins_b, 7);
  XFUZZ2(xvextrins_b, 15);
  XFUZZ2(xvextrins_b, 16);
  XFUZZ2(xvextrins_b, 32);
  XFUZZ2(xvextrins_b, 64);
  XFUZZ2(xvextrins_b, 128);
  XFUZZ2(xvextrins_b, 255);
}
