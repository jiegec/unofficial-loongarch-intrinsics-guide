#include "common.h"

v128 vinsgr2vr_d(v128 a, long int b, int imm) {
  v128 dst;
#include "vinsgr2vr_d.h"
  return dst;
}

void test() {
  FUZZ1(vinsgr2vr_d, 0, 0);
  FUZZ1(vinsgr2vr_d, 1234, 1);
  FUZZ1(vinsgr2vr_d, 5678, 1);
}
