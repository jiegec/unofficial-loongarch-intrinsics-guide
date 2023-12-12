#include "common.h"

v128 vaddi_wu(v128 a, int imm) {
  v128 dst;
#include "vaddi_wu.h"
  return dst;
}

void test() {
  FUZZ1(vaddi_wu, 0);
  FUZZ1(vaddi_wu, 31);
}
