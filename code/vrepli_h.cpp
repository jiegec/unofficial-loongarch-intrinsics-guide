#include "common.h"

v128 vrepli_h(int imm) {
  v128 dst;
#include "vrepli_h.h"
  return dst;
}

void test() {
  FUZZ0(vrepli_h, 0);
  FUZZ0(vrepli_h, 1);
  FUZZ0(vrepli_h, 511);
}
