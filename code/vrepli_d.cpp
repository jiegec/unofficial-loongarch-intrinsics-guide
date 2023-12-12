#include "common.h"

v128 vrepli_d(int imm) {
  v128 dst;
#include "vrepli_d.h"
  return dst;
}

void test() {
  FUZZ0(vrepli_d, 0);
  FUZZ0(vrepli_d, 1);
  FUZZ0(vrepli_d, 511);
}
