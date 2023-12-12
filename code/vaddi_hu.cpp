#include "common.h"

v128 vaddi_hu(v128 a, int imm) {
  v128 dst;
#include "vaddi_hu.h"
  return dst;
}

void test() {
  FUZZ1(vaddi_hu, 0);
  FUZZ1(vaddi_hu, 31);
}
