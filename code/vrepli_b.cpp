#include "common.h"

v128 vrepli_b(int imm) {
  v128 dst;
#include "vrepli_b.h"
  return dst;
}

void test() {
  FUZZ0(vrepli_b, 0);
  FUZZ0(vrepli_b, 1);
  FUZZ0(vrepli_b, 511);
}
