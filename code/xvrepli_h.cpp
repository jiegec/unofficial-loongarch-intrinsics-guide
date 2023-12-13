#include "common.h"

v256 xvrepli_h(int imm) {
  v256 dst;
#include "xvrepli_h.h"
  return dst;
}

void test() {
  XFUZZ0(xvrepli_h, 0);
  XFUZZ0(xvrepli_h, 1);
  XFUZZ0(xvrepli_h, 511);
}
