#include "common.h"

v256 xvrepli_w(int imm) {
  v256 dst;
#include "xvrepli_w.h"
  return dst;
}

void test() {
  XFUZZ0(xvrepli_w, 0);
  XFUZZ0(xvrepli_w, 1);
  XFUZZ0(xvrepli_w, 511);
}
