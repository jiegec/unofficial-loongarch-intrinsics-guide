#include "common.h"

v128 vslli_w(v128 a, int imm) {
  v128 dst;
#include "vslli_w.h"
  return dst;
}

void test() {
  FUZZ1(vslli_w, 0);
  FUZZ1(vslli_w, 7);
}
