#include "common.h"

v128 vmskltz_h(v128 a) {
  v128 dst;
#include "vmskltz_h.h"
  return dst;
}

void test() { FUZZ1(vmskltz_h); }
