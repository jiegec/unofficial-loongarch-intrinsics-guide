#include "common.h"

v128 vmskltz_d(v128 a) {
  v128 dst;
#include "vmskltz_d.h"
  return dst;
}

void test() { FUZZ1(vmskltz_d); }
