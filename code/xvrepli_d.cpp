#include "common.h"

v256 xvrepli_d(int imm) {
  v256 dst;
#include "xvrepli_d.h"
  return dst;
}

void test() {
  XFUZZ0(xvrepli_d, 0);
  XFUZZ0(xvrepli_d, 1);
  XFUZZ0(xvrepli_d, 511);
}
