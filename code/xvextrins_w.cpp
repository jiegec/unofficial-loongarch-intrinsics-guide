#include "common.h"

v256 xvextrins_w(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvextrins_w.h"
  return dst;
}

void test() {
  XFUZZ2(xvextrins_w, 0);
  XFUZZ2(xvextrins_w, 3);
  XFUZZ2(xvextrins_w, 7);
  XFUZZ2(xvextrins_w, 15);
  XFUZZ2(xvextrins_w, 16);
  XFUZZ2(xvextrins_w, 32);
  XFUZZ2(xvextrins_w, 64);
  XFUZZ2(xvextrins_w, 128);
  XFUZZ2(xvextrins_w, 255);
}
