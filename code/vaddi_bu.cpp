#include "common.h"

v128 vaddi_bu(v128 a, int imm) {
  v128 dst;
#include "vaddi_bu.h"
  return dst;
}

void test() {
  FUZZ1(vaddi_bu, 0);
  FUZZ1(vaddi_bu, 31);
}
