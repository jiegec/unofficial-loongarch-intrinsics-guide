#include "common.h"

v256 xvperm_w(v256 a, v256 b) {
  v256 dst;
#include "xvperm_w.h"
  return dst;
}

void test() {
  XFUZZ2(xvperm_w);
  XFUZZ2(xvperm_w);
  XFUZZ2(xvperm_w);
}
