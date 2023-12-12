#include "common.h"

v128 vinsgr2vr_w(v128 a, long int b, int imm) {
  v128 dst;
#include "vinsgr2vr_w.h"
  return dst;
}

void test() {
  FUZZ1(vinsgr2vr_w, 0, 0);
  FUZZ1(vinsgr2vr_w, 1234, 1);
  FUZZ1(vinsgr2vr_w, 5678, 1);
}
