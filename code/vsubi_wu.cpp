#include "common.h"

v128 vsubi_wu(v128 a, int imm) {
  v128 dst;
#include "vsubi_wu.h"
  return dst;
}

void test() {
  FUZZ1(vsubi_wu, 0);
  FUZZ1(vsubi_wu, 31);
}
