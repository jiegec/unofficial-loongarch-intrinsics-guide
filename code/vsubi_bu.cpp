#include "common.h"

v128 vsubi_bu(v128 a, int imm) {
  v128 dst;
#include "vsubi_bu.h"
  return dst;
}

void test() {
  FUZZ1(vsubi_bu, 0);
  FUZZ1(vsubi_bu, 31);
}
