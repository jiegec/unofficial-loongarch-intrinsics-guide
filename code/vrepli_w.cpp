#include "common.h"

v128 vrepli_w(int imm) {
  v128 dst;
#include "vrepli_w.h"
  return dst;
}

void test() {
  FUZZ0(vrepli_w, 0);
  FUZZ0(vrepli_w, 1);
  FUZZ0(vrepli_w, 511);
}
