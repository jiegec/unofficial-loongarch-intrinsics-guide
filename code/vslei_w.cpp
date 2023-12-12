#include "common.h"

v128 vslei_w(v128 a, int imm) {
  v128 dst;
#include "vslei_w.h"
  return dst;
}

void test() {
  FUZZ1(vslei_w, 0);
  FUZZ1(vslei_w, 15);
}
