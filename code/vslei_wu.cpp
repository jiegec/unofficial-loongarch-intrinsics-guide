#include "common.h"

v128 vslei_wu(v128 a, int imm) {
  v128 dst;
#include "vslei_wu.h"
  return dst;
}

void test() {
  FUZZ1(vslei_wu, 0);
  FUZZ1(vslei_wu, 15);
}
