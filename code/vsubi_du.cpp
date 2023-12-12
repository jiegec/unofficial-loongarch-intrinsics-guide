#include "common.h"

v128 vsubi_du(v128 a, int imm) {
  v128 dst;
#include "vsubi_du.h"
  return dst;
}

void test() {
  FUZZ1(vsubi_du, 0);
  FUZZ1(vsubi_du, 31);
}
