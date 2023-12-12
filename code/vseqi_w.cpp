#include "common.h"

v128 vseqi_w(v128 a, int imm) {
  v128 dst;
#include "vseqi_w.h"
  return dst;
}

void test() {
  FUZZ1(vseqi_w, -16);
  FUZZ1(vseqi_w, 0);
  FUZZ1(vseqi_w, 15);
}
