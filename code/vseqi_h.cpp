#include "common.h"

v128 vseqi_h(v128 a, int imm) {
  v128 dst;
#include "vseqi_h.h"
  return dst;
}

void test() {
  FUZZ1(vseqi_h, -16);
  FUZZ1(vseqi_h, 0);
  FUZZ1(vseqi_h, 15);
}
