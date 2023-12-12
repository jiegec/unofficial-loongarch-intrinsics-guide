#include "common.h"

v128 vinsgr2vr_h(v128 a, long int b, int imm) {
  v128 dst;
#include "vinsgr2vr_h.h"
  return dst;
}

void test() {
  FUZZ1(vinsgr2vr_h, 0, 0);
  FUZZ1(vinsgr2vr_h, 1234, 1);
  FUZZ1(vinsgr2vr_h, 5678, 1);
}
