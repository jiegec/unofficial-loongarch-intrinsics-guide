#include "common.h"

v128 vseqi_b(v128 a, int imm) {
  v128 dst;
#include "vseqi_b.h"
  return dst;
}

void test() {
  FUZZ1(vseqi_b, -16);
  FUZZ1(vseqi_b, 0);
  FUZZ1(vseqi_b, 15);
}
