#include "common.h"

v256 xvpermi_w(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvpermi_w.h"
  return dst;
}

void test() {
  XFUZZ2(xvpermi_w, 0);
  XFUZZ2(xvpermi_w, 1);
  XFUZZ2(xvpermi_w, 2);
  XFUZZ2(xvpermi_w, 0xFF);
}
