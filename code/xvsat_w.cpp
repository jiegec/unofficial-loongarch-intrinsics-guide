#include "common.h"

v256 xvsat_w(v256 a, int imm) {
  v256 dst;
#include "xvsat_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvsat_w, 0);
  XFUZZ1(xvsat_w, 7);
}
