#include "common.h"

v128 vaddi_du(v128 a, int imm) {
  v128 dst;
#include "vaddi_du.h"
  return dst;
}

void test() {
  FUZZ1(vaddi_du, 0);
  FUZZ1(vaddi_du, 31);
}
