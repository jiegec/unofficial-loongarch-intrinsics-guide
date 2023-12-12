#include "common.h"

v128 vmskltz_b(v128 a) {
  v128 dst;
#include "vmskltz_b.h"
  return dst;
}

void test() { FUZZ1(vmskltz_b); }
