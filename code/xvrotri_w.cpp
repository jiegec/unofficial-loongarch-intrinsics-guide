#include "common.h"

v256 xvrotri_w(v256 a, int imm) {
  v256 dst;
#include "xvrotri_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvrotri_w, 0);
  XFUZZ1(xvrotri_w, 7);
}
