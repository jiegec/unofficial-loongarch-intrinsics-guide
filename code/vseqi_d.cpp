#include "common.h"

v128 vseqi_d(v128 a, int imm) {
  v128 dst;
#include "vseqi_d.h"
  return dst;
}

void test() {
  FUZZ1(vseqi_d, -16);
  FUZZ1(vseqi_d, 0);
  FUZZ1(vseqi_d, 15);
}
