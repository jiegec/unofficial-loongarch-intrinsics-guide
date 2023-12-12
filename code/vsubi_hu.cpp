#include "common.h"

v128 vsubi_hu(v128 a, int imm) {
  v128 dst;
#include "vsubi_hu.h"
  return dst;
}

void test() {
  FUZZ1(vsubi_hu, 0);
  FUZZ1(vsubi_hu, 31);
}
