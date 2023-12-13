#include "common.h"

v256 xvrepli_b(int imm) {
  v256 dst;
#include "xvrepli_b.h"
  return dst;
}

void test() {
  XFUZZ0(xvrepli_b, 0);
  XFUZZ0(xvrepli_b, 1);
  XFUZZ0(xvrepli_b, 511);
}
