#include "common.h"

v128 vmsknz_b(v128 a) {
  v128 dst;
#include "vmsknz_b.h"
  return dst;
}

void test() { FUZZ1(vmsknz_b); }
