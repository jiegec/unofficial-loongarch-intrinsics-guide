#include "common.h"

v128 vmskltz_w(v128 a) {
  v128 dst;
#include "vmskltz_w.h"
  return dst;
}

void test() { FUZZ1(vmskltz_w); }
