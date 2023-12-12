#include "common.h"

v128 vinsgr2vr_b(v128 a, long int b, int imm) {
  v128 dst;
#include "vinsgr2vr_b.h"
  return dst;
}

void test() {
  FUZZ1(vinsgr2vr_b, 0, 0);
  FUZZ1(vinsgr2vr_b, 1234, 1);
  FUZZ1(vinsgr2vr_b, 5678, 1);
}
